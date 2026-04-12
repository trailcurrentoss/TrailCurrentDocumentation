# TrailCurrent Data Flow

Detailed data movement through the TrailCurrent system across different scenarios.

## Typical Data Flow Paths

### Scenario 1: Sensor Data Reading (Air Quality — Borealis Module)

```
┌──────────────────┐
│ SGP30 VOC Sensor │
│ + SHT31 Temp/RH  │
└──────┬───────────┘
       │ Reads periodically
       │
       ▼
┌──────────────┐      CAN ID: 0x01f
│  Borealis    │      Payload: [tempC, tempF, humH, humL, tvocH, tvocL, eco2H, eco2L]
│  (ESP32)     │
└──────┬───────┘
       │
       ▼ CAN Bus (500 kbps)
┌─────────────────────┐
│ In-Vehicle Compute  │
│ CAN Bridge decodes  │
│ CAN ID 0x01f        │
└──────┬──────────────┘
       │
       ▼ Publishes structured JSON to local MQTT
┌──────────────────────────────────────────────────────────────┐
│ Local MQTT Broker (Mosquitto)                                │
│ Topic: local/airquality/temphumid                            │
│   {"tempInC": 22, "tempInF": 72, "humidity": 45.50}         │
│ Topic: local/airquality/status                               │
│   {"tvoc_ppb": 150, "eco2_ppm": 420}                        │
└──────┬───────────────────────────────────────────────────────┘
       │
       ├─→ Local frontend (Spotter PWA, real-time via WebSocket)
       │
       └─→ Cloud Bridge (if connected to Farwatch)
            │  Tier: Slow (15s interval, change-only)
            │  Only forwards if value changed AND 15s elapsed
            ▼
         Cloud MQTT Broker (Farwatch, MQTTS over cellular)
         Topic: rv/airquality/temphumid
         Topic: rv/airquality/status
            │
            ▼
         Farwatch Backend → WebSocket fan-out (no DB storage)
            │
            ▼
         Browser (Farwatch dashboard)
         Displays: 22°C, 45.5% RH, 150 ppb TVOC
```

### Scenario 2: User Command (Remote Light Toggle via Farwatch)

Light and relay commands use **toggle semantics** — the physical hardware module
(Ampline PDM / Switchback) is the single source of truth. Multiple controllers
(physical switches, touch screens, local PWA, Farwatch) all send toggle commands,
and the device broadcasts its actual state to all listeners.

```
┌─────────────────────┐
│  Farwatch Dashboard │
│  User taps light 3  │
│  toggle button      │
└────────┬────────────┘
         │
         ▼ HTTPS PUT /api/lights/3
┌─────────────────────┐
│  Farwatch Backend   │
│  Publishes MQTT:    │
│  rv/lights/3/command│
│  {"state": 1}       │
└────────┬────────────┘
         │
         ▼ MQTTS over cellular (port 8883)
┌─────────────────────┐
│ Cloud Bridge        │
│ (on vehicle)        │
│ Receives command,   │
│ sends CAN 0x018     │
│ [0x02] (toggle #3)  │
└────────┬────────────┘
         │
         ▼ CAN Bus (500 kbps)
┌──────────────────┐
│ Ampline PDM      │
│ (ESP32)          │
│ Receives toggle, │
│ flips light 3    │
└────────┬─────────┘
         │
         ▼ Broadcasts new state: CAN ID 0x01b
         │ (all 8 light states in one frame)
         │
┌────────────────────────────────────────────────────┐
│ CAN Bridge decodes 0x01b → 8 MQTT publishes:      │
│ local/lights/1/status  {"state":0,"brightness":0}  │
│ local/lights/2/status  {"state":1,"brightness":255}│
│ local/lights/3/status  {"state":1,"brightness":255}│  ← changed!
│ ...                                                 │
└────────┬───────────────────────────────────────────┘
         │
         ├─→ Local UI (Spotter PWA) — sees change instantly
         │
         └─→ Cloud Bridge
              │  Tier: Immediate (on change only)
              │  light 3 changed → forwards immediately
              │  lights 1,2,4-8 unchanged → suppressed
              ▼
           Cloud MQTT: rv/lights/3/status {"state":1,"brightness":255}
              │
              ▼
           Farwatch Backend → WebSocket → Browser
           Dashboard updates: light 3 now shows ON
```

### Scenario 3: Offline Operation (No Cloud)

The system is fully autonomous without cloud connectivity. All local control,
sensor reading, and UI functions continue normally. The cloud bridge simply
has no connected cloud client, so `handleLocalMessage` returns early.

```
Hardware Module      In-Vehicle Compute       Cloud
       │                  │                      │
       │─ CAN Message ───→│                      │
       │                  │ Cloud bridge:         │
       │                  │ cloudClient=null,     │
       │                  │ skips forwarding      │ (unreachable)
       │                  │                      │
       │                  │ Local MQTT still      │
       │                  │ delivers to Spotter   │
       │                  │ PWA and local UI      │
       │                  │                      │
       │◄─ CAN Command ──│ (local toggle works)  │
       │                  │                      │
      (all local functions operate normally)

When cloud connectivity returns:
  1. Cloud bridge reconnects (mqtts://, 5s retry interval)
  2. on('connect') fires → forced heartbeat publishes ALL cached state
  3. system_sync_trigger re-publishes full system config
  4. Farwatch dashboard immediately shows current vehicle state
  5. No queued commands — Farwatch was unreachable, so no commands were sent
```

## Message Routing Examples

### Example 1: Milepost (GNSS) Position Update

```
Milepost (GNSS Module, ESP32)
  ├─ GPS fix available (~1 Hz)
  ├─ CAN Messages (4 frames):
  │  - 0x009: [latSign, lat2, lat1, lat0, lonSign, lon2, lon1, lon0]
  │           Sign-magnitude encoding, divided by 10000 for degrees
  │  - 0x008: [alt3, alt2, alt1, alt0, ...]  (centimeters, unsigned)
  │  - 0x007: [satCount, sogH, sogL, cogH, cogL, gnssMode, ...]
  │  - 0x006: [yearH, yearL, month, day, hour, minute, second, ...]
  └─ Sends all 4 frames via CAN bus

CAN Bridge (on In-Vehicle Compute)
  ├─ Receives CAN 0x009, decodes sign-magnitude lat/lon
  ├─ Publishes: local/gps/latlon  {"latitude": 35.6892, "longitude": -105.9371}
  ├─ Receives CAN 0x008, converts centimeters to meters/feet
  ├─ Publishes: local/gps/alt     {"altitudeInMeters": 2134.56, "altitudeFeet": 7002}
  ├─ Receives CAN 0x007
  ├─ Publishes: local/gps/details {"numberOfSatellites": 12, "speedOverGround": 0, ...}
  ├─ Receives CAN 0x006
  └─ Publishes: local/gps/time    {"year": 2026, "month": 4, "day": 7, ...}

Cloud Bridge
  ├─ local/gps/latlon → rv/gps/latlon
  │  Tier: Standard (5s interval, change-only, GPS threshold bypass)
  │  Parked vehicle: same position → suppressed by change detection
  │  Moving >50m: threshold bypass → forwards immediately
  ├─ local/gps/alt → rv/gps/alt
  │  Tier: Slow (15s interval, change-only)
  ├─ local/gps/details → rv/gps/details
  │  Tier: Slow (15s interval, change-only)
  └─ local/gps/time → NOT forwarded (eliminated, Farwatch has own clock)

Farwatch Backend
  ├─ Receives rv/gps/latlon on MQTT
  ├─ No database storage (real-time only)
  ├─ Broadcasts WebSocket: {type: "latlon", data: {latitude, longitude}}
  └─ Used by proximity engine for distance calculations

Farwatch Dashboard
  ├─ Receives WebSocket "latlon" event
  ├─ Updates map marker position
  └─ 30-second staleness timeout — marks data stale if no update received
```

### Example 2: Energy Data (Merged from 3 CAN Frames)

```
Ampline PDM (battery monitoring via Victron VE.Direct → CAN)
  ├─ CAN 0x023: [voltH, voltL, _, _, _, percentH, percentL, _]
  │  battery_voltage = voltH + (voltL / 100)
  │  battery_percent = percentH + (percentL / 100)
  │
  ├─ CAN 0x024: [negFlag, wattH, wattL, ttgH, ttgL, _, _, _]
  │  consumption_watts (if negFlag=0xFF, discharging)
  │  time_remaining_minutes
  │
  └─ CAN 0x02c: [_, _, solarH, solarL, _, _, csEnum, _]
     solar_watts, charge_type (off/bulk/absorption/float/equalize)

CAN Bridge (energy merge accumulator)
  ├─ Each CAN frame updates a shared energyState object
  ├─ Object.assign(energyState, { battery_voltage: 13.12, ... })
  ├─ Publishes merged result after EACH frame arrival:
  │  local/energy/status
  │  {"battery_voltage": 13.12, "battery_percent": 87.50,
  │   "consumption_watts": 450, "solar_watts": 380,
  │   "charge_type": "absorption", "time_remaining_minutes": 720}
  └─ Result: 3 publishes/sec of the same merged object (one per CAN frame)

Cloud Bridge
  ├─ Tier: Standard (5s interval, change-only, energy threshold bypass)
  ├─ First publish: forwards immediately (no previous value)
  ├─ Next 14 publishes (5 seconds): identical JSON → suppressed by change detection
  ├─ At 5s mark: if value changed, forwards; if unchanged, suppressed
  ├─ Threshold bypass: voltage >0.5V, solar >50W, percent >2%, charge_type changes
  │  → forwards immediately regardless of interval
  └─ Result: ~1 msg every 5s instead of 3 msgs/sec (93% reduction)

Farwatch Backend
  ├─ Receives rv/energy/status
  ├─ No database storage
  └─ WebSocket broadcast: {type: "energy", data: {solar_watts, battery_percent, ...}}
```

## Data Storage Locations

### In-Vehicle Compute (Edge — Local Storage)
```
MongoDB: mongodb://mongodb:27017 (Docker volume: mongodb-data)
├─ system_config     — MCU modules, WiFi, cloud credentials (singleton)
├─ lights            — PDM channel metadata (name, icon, type)
├─ settings          — User preferences (theme)
└─ firmware_cache    — OTA deployment state

Note: Sensor data (energy, GPS, air quality, level) is NOT stored locally.
It flows through MQTT in real-time only.
```

### Cloud Storage (Farwatch — MongoDB)
```
MongoDB: mongodb://mongodb:27017/trailcurrent
├─ lights              — Light/relay metadata synced from vehicle config
├─ system_config       — Latest vehicle config snapshot (singleton)
├─ settings            — User preferences (theme)
├─ users / api_keys    — Authentication
├─ proximity_devices   — Registered phone devices + last location
├─ proximity_config    — Zone radii, hysteresis, debounce (singleton)
├─ proximity_rules     — Automation rules (trigger → actions)
├─ deployments         — OTA package metadata
└─ deployment_statuses — Deployment progress tracking

Note: Sensor data (energy, GPS, air quality, level, thermostat) is NOT
stored in the database. It is received via MQTT and immediately broadcast
to connected browsers via WebSocket. There is no historical data retention
for sensor readings — Farwatch is a real-time dashboard only.
```

## Message Frequency & Volume

### CAN Bus (Local — Device Tier to Edge Tier)

CAN modules broadcast at hardware-determined rates (typically 1–10 Hz). The CAN bridge on the edge tier decodes frames by CAN ID and publishes structured JSON to local MQTT topics. There is no rate limiting on the local bus — in-vehicle operation is real-time.

| CAN ID(s) | Data Type | Module | Local MQTT Topics | Payload Fields | Approx. Size |
|---|---|---|---|---|---|
| 0x01b | Light status | Ampline (PDM) | `local/lights/1-8/status` (8 topics) | state, brightness | ~40 B each |
| 0x028/029/02a | Relay status | Switchback (3 instances) | `local/relays/1-24/status` (24 topics) | state | ~30 B each |
| 0x023, 0x024, 0x02c | Energy | Ampline / Inverter | `local/energy/status` (merged) | battery_voltage, battery_percent, consumption_watts, solar_watts, charge_type, time_remaining_minutes | ~115 B |
| 0x009 | GPS position | Milepost | `local/gps/latlon` | latitude, longitude (4 decimal places, ~11m precision) | ~55 B |
| 0x008 | GPS altitude | Milepost | `local/gps/alt` | altitudeInMeters, altitudeFeet | ~65 B |
| 0x007 | GNSS stats | Milepost | `local/gps/details` | numberOfSatellites, speedOverGround, courseOverGround, gnssMode | ~70 B |
| 0x006 | GPS time | Milepost | `local/gps/time` | year, month, day, hour, minute, second | ~60 B |
| 0x01f | Air quality | Borealis | `local/airquality/status`, `local/airquality/temphumid` | tvoc_ppb, eco2_ppm, tempInC, tempInF, humidity | ~40–60 B each |
| 0x030 | Leveling | Plateau | `local/level/tilt` | front_back, side_to_side, front_back_diff_mm, left_right_diff_mm | ~90 B |
| 0x032 | IMU status | Plateau | `local/level/status` | imu_connected, fully_calibrated, cal_sys/gyro/accel/mag, mounting | ~85 B |
| — | System stats | Edge compute | `local/system/stats` (every 10s) | cpu_temp_c, cpu_percent, fan_percent | ~50 B |

### Cloud Sync (Edge Tier to Cloud Tier)

The cloud bridge (`cloud-bridge.js`) selectively forwards local MQTT data to the Farwatch cloud broker over cellular LTE (MQTTS on port 8883). To stay within cellular data budgets (~10 GB/month per vehicle), the bridge uses three mechanisms:

1. **Change detection** — Messages identical to the last-sent value are suppressed. This eliminates the vast majority of traffic since lights, relays, and most sensors broadcast unchanged state every second.

2. **Tiered intervals** — Each data type has a minimum interval between sends, based on how quickly the underlying value actually changes:

| Tier | Interval | Cloud Topics | Rationale |
|---|---|---|---|
| Immediate | On change only | `rv/lights/*/status`, `rv/relays/*/status`, `rv/thermostat/status` | User-facing state needs instant feedback |
| Standard | 5 seconds | `rv/energy/status`, `rv/gps/latlon` | Changes slowly; energy has 3 CAN frames/sec publishing the same merged object |
| Slow | 15 seconds | `rv/gps/alt`, `rv/gps/details`, `rv/airquality/*`, `rv/level/*` | Rarely changes when vehicle is parked |
| Background | 30 seconds | `rv/system/stats` | Diagnostic only |
| Eliminated | Never sent | GPS time | Farwatch has its own clock |

3. **Threshold bypass** — Standard-tier topics (energy, GPS) bypass their interval and send immediately when values change significantly:
   - Energy: voltage >0.5V, solar >50W, battery percent >2%, or charge_type changes
   - GPS: position moves >0.0005° (~50m)

4. **Heartbeat** — Every 20 seconds, the bridge republishes all last-known state regardless of changes. This is a safety net for connection-level failures where QoS 1 retry couldn't complete (e.g., TCP dropped between vehicle and cloud broker). It bounds maximum cloud desync to 20 seconds. In normal operation, state changes are forwarded immediately.

### Estimated Cloud Data Usage

| Scenario | Messages/sec | Monthly (30 days, 24/7) |
|---|---|---|
| Without optimization (all data forwarded at CAN rate) | ~41 | ~9–13 GB |
| Optimized — steady state (vehicle parked, no changes) | ~0.7 (heartbeat only) | ~0.3 GB |
| Optimized — active use (occasional toggles) | ~2–3 | ~1–2 GB |
| Optimized — worst case (constant state changes) | ~10–15 | ~3–5 GB |

### Cloud Data Flow Summary

```
CAN Bus (hardware rates)
    ↓
CAN Bridge (decode, publish to local MQTT)
    ↓
Local MQTT Broker (real-time, no filtering)
    ↓
Cloud Bridge (change detection + tiered intervals + heartbeat)
    ↓  Only changed or interval-due messages
Cloud MQTT Broker (Farwatch, MQTTS over cellular)
    ↓
Farwatch Backend (WebSocket fan-out, no historical storage)
    ↓
Browser (real-time dashboard, 30s staleness timeout)
```

**Important design note:** Light and relay commands use **toggle semantics** (CAN 0x018 for lights, 0x025+ for relays), not explicit SET commands. The physical hardware module (Ampline PDM / Switchback) is the single source of truth. This enables multiple controllers (physical switches, touch screens, local PWA, Farwatch cloud) to coexist without race conditions — every controller toggles, and the device broadcasts actual state to all listeners.

---

See also:
- [SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md) - How components fit together
- [NETWORK_TOPOLOGY.md](NETWORK_TOPOLOGY.md) - Network structure
- [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) - CAN message definitions
- [10_Reference/MQTT_TOPICS.md](../10_Reference/MQTT_TOPICS.md) - MQTT topic structure
