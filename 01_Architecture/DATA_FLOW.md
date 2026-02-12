# TrailCurrent Data Flow

Detailed data movement through the TrailCurrent system across different scenarios.

## Typical Data Flow Paths

### Scenario 1: Sensor Data Reading (Normal Operation)

```
┌─────────────┐
│ Temperature │
│   Sensor    │
└──────┬──────┘
       │ Reads temperature every 10s
       │
       ▼
┌──────────────┐      CAN ID: 0x123
│  Temp Module │      Message: [TEMP_DATA, value=25.3°C]
│ (ESP32)      │
└──────┬───────┘
       │
       ▼ CAN Bus (500 kbps)
┌─────────────────────┐
│ In-Vehicle Compute  │
│ CAN Transceiver     │
└──────┬──────────────┘
       │
       ▼ Converts to MQTT
┌─────────────────────┐
│ MQTT Broker         │
│ Topic: tc/temp/     │
│ current_temp        │
└──────┬──────────────┘
       │
       ├─→ Container 1: Logger (stores in SQLite)
       ├─→ Container 2: API Server (caches in memory)
       └─→ Cloud MQTT Client (if connected)
            │
            └─→ Cloud Server
                │
                ├─→ REST API (query endpoint: /api/sensors/temp)
                ├─→ Database (PostgreSQL)
                └─→ WebSocket (real-time to connected clients)
                     │
                     ▼
                  Browser / Mobile App
                  Displays: 25.3°C with timestamp
```

### Scenario 2: User Command (Remote Control)

```
┌─────────────────┐
│  Web Dashboard  │
│  User clicks    │
│  "Turn On"      │
└────────┬────────┘
         │
         ▼ HTTPS Request
┌─────────────────┐      POST /api/devices/power/on
│   Cloud API     │      with auth token
└────────┬────────┘
         │
         ▼ Validate auth, check permissions
┌─────────────────┐
│ MQTT Client     │
│ (Publish)       │
└────────┬────────┘
         │
         ▼ MQTT Topic: tc/command/power
│         Message: {action: "on", timestamp: xxx}
│
├─────────────────────────────────────────┐
│        Internet (if available)          │
└─────────────────────────────────────────┘
         │
         ▼
┌──────────────────┐
│ In-Vehicle       │
│ MQTT Subscriber  │
└────────┬─────────┘
         │
         ▼ Convert MQTT to CAN
┌──────────────────┐
│ CAN Bus (500kbps)│      Message: POWER_ON_CMD
└────────┬─────────┘      CAN ID: 0x200
         │
         ▼
┌──────────────────┐
│ Power Module     │
│ (ESP32)          │
│ Receives command │
└────────┬─────────┘
         │
         ▼ Execute (toggle relay)
┌──────────────────┐
│ Power Relay      │
│ Turns ON         │
└────────┬─────────┘
         │
         ▼ Status change → CAN Status Update
┌──────────────────┐
│ Power Module     │
│ Sends ACK        │
│ CAN ID: 0x280    │
└────────┬─────────┘
         │
         ▼ Cycles back through Pi → Cloud → UI
         │ (confirms to user: "Power ON")
         ▼
```

### Scenario 3: Offline Operation (No Cloud)

```
Hardware Module      In-Vehicle Compute  Cloud
       │                  │                │
       │─ CAN Message ────→│                │
       │                   │ No internet    │
       │                   ├─ Cache data   │
       │                   ├─ Store locally│
       │                   │                │
       │◄─ CAN Response ───│                │
       │                   │ Queue commands│
       │                   │                │
      (continues operating normally)

When cloud returns online:
Pi syncs cache → Cloud
Cloud processes stored commands
```

## Message Routing Examples

### Example 1: GPS Position Update

```
GPS Module
  ├─ Interval: 5 seconds
  ├─ CAN Message Format:
  │  - Frame ID: 0x100
  │  - Payload: [LAT_H, LAT_L, LON_H, LON_L, SAT_COUNT, HDOP]
  └─ Sends via CAN

In-Vehicle Compute
  ├─ Receives CAN 0x100
  ├─ Parses coordinates: 47.2530°N, 122.4475°W
  ├─ Publishes MQTT:
  │  - Topic: tc/gps/position
  │  - Payload: {"lat": 47.2530, "lon": -122.4475, "satellites": 12, "timestamp": 1234567890}
  └─ Sends to cloud (if connected)

Cloud Application
  ├─ Receives MQTT: tc/gps/position
  ├─ Stores in PostgreSQL table: gps_readings
  ├─ Updates Redis cache: last_known_position
  ├─ Publishes WebSocket to connected users
  │  - Event: position_updated
  │  - Data: {lat, lon, timestamp, accuracy}
  └─ Available via API: GET /api/location/current

User Dashboard
  ├─ Receives WebSocket update
  ├─ Updates map marker position
  ├─ Shows timestamp: "2 seconds ago"
  └─ User sees live location tracking
```

### Example 2: Power Control Sequence

```
User clicks "Turn Heater On" in web interface

Cloud API
  ├─ Validates: user permissions, device state
  ├─ Creates command entry in DB (queued state)
  ├─ Publishes MQTT: tc/command/heater
  │  Payload: {"id": "cmd_123", "action": "on", "priority": "normal"}
  └─ Returns 202 Accepted to UI

In-Vehicle Compute (MQTT Subscriber)
  ├─ Receives: tc/command/heater
  ├─ Validates command signature
  ├─ Converts to CAN frame:
  │  - Frame ID: 0x320 (heater commands)
  │  - Byte 0: 0x01 (turn on)
  │  - Bytes 1-7: [command_id, checksum, ...]
  └─ Sends via CAN bus

Heater Control Module
  ├─ Receives CAN 0x320
  ├─ Validates checksum
  ├─ Checks safety interlocks (is ignition on? etc.)
  ├─ If safe:
  │  ├─ Activates heating element
  │  ├─ Sends status CAN 0x380: [0x01, temp, power, ...]
  │  ├─ Continues monitoring temperature
  │  └─ Throttles if temperature too high
  └─ If unsafe:
     └─ Sends error status CAN 0x380: [0xFF, error_code]

In-Vehicle Compute (monitoring status)
  ├─ Receives CAN 0x380
  ├─ Publishes MQTT: tc/status/heater
  │  Payload: {"state": "on", "temperature": 45.2, "power_watts": 1200}
  └─ Sends to cloud

Cloud Application
  ├─ Updates device state in DB
  ├─ Updates command entry: success
  ├─ Publishes WebSocket: heater_status_changed
  └─ Sends notification to UI

User Dashboard
  ├─ Receives WebSocket update
  ├─ Updates UI: heater icon shows "ON"
  ├─ Shows temperature: 45.2°C
  ├─ Shows power consumption: 1200W
  └─ User sees confirmation: "Heater turned on successfully"
```

## Data Storage Locations

### In-Vehicle Compute (Local Storage)
```
SQLite Database: /var/lib/trailcurrent/data.db
├─ Last 24 hours of sensor data
├─ Command queue
├─ Device states
└─ Configuration

File System: /etc/trailcurrent/
├─ Device configuration
├─ CAN ID mappings
├─ MQTT topic definitions
└─ Credentials
```

### Cloud Storage
```
PostgreSQL Database:
├─ Historical sensor data (long-term)
├─ User accounts and permissions
├─ Device registry and metadata
├─ Command history
├─ Alerts and notifications
└─ Analytics/reports

Redis Cache:
├─ Last known positions
├─ Current device states
├─ Session tokens
└─ Rate limiting

File Storage:
├─ Firmware binaries (for OTA)
├─ Analytics reports
├─ Photos/media
└─ Backups
```

## Message Frequency & Volume

| Data Type | Frequency | Size | Network Impact |
|-----------|-----------|------|-----------------|
| GPS Position | 5s | 15 bytes CAN | ~24 kbps |
| Temperature | 30s | 8 bytes CAN | ~2 kbps |
| Power Data | 10s | 12 bytes CAN | ~10 kbps |
| Status Updates | 60s | 8 bytes CAN | ~1 kbps |
| Commands | Event-based | varies | ~1 kbps avg |
| **Total CAN** | - | - | **~38 kbps** |
| **Total MQTT** | - | - | **~50 kbps** |
| **Cloud Sync** | 1-5s | JSON (30-50B) | **~0.5-5 Mbps** |

---

See also:
- [SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md) - How components fit together
- [NETWORK_TOPOLOGY.md](NETWORK_TOPOLOGY.md) - Network structure
- [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) - CAN message definitions
- [10_Reference/MQTT_TOPICS.md](../10_Reference/MQTT_TOPICS.md) - MQTT topic structure
