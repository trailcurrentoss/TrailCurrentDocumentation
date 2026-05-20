# TrailCurrent Network Topology

Network and communication architecture for the TrailCurrent platform.

## Physical Network Layout

```
┌──────────────────────────────────────────────────────┐
│                    INTERNET                           │
│            (Public Network / Cloud)                   │
└────────────────┬─────────────────────────────────────┘
                 │ WiFi/Ethernet (Secured)
                 │
         ┌───────▼──────┐
         │ Firewall/NAT │
         └───────┬──────┘
                 │
┌────────────────▼──────────────────────────────────────┐
│         VEHICLE NETWORK (Private)                      │
│                                                        │
│  ┌──────────────────────────────────────────────┐    │
│  │  In-Vehicle Compute (192.168.x.x)           │    │
│  │  (Raspberry Pi CM5 + Waveshare CAN HAT)      │    │
│  │  ├─ Docker Network                           │    │
│  │  ├─ MQTT Broker (localhost:1883)             │    │
│  │  ├─ REST API (localhost:3000)                │    │
│  │  └─ MCP2515 CAN Interface (SPI)              │    │
│  └──┬───────────────────────────────────────────┘    │
│     │                                                 │
│     └─────────────────────┬──────────────────────────┤
│                           │ CAN Bus (Isolated)       │
│  ┌────────────────────────▼────────────────────────┐ │
│  │ CAN Bus Network (All modules)                   │ │
│  │ Bus Speed: 500 kbps or 1 Mbps                   │ │
│  │                                                  │ │
│  │  ┌────────┐  ┌────────┐  ┌────────┐             │ │
│  │  │Module A│  │Module B│  │Module C│    ...      │ │
│  │  │(ID:xx) │  │(ID:yy) │  │(ID:zz) │             │ │
│  │  └────────┘  └────────┘  └────────┘             │ │
│  └──────────────────────────────────────────────────┘ │
│                                                        │
└────────────────────────────────────────────────────────┘

Secondary Communication Paths (Optional):
├─ Bluetooth: Direct module ↔ mobile
├─ WiFi: Direct module ↔ cloud
└─ EspNow: Direct module ↔ module
```

## CAN Bus Topology

### Ring Bus (Recommended)
```
┌─────────────┐
│   Pi (CAN)  │
└──────┬──────┘
       │
     CAN
       │
   ┌───┴────┐
   │        │
 ┌─▼─┐    ┌─▼─┐
 │ M1│    │ M2│
 └─┬─┘    └─┬─┘
   │        │
   └────┬───┘
        │
      CAN
        │
    ┌───┴────┐
    │        │
  ┌─▼─┐    ┌─▼─┐
  │ M3│    │ M4│
  └────┘    └────┘
```

### Star Bus (Alternative)
```
           ┌──────────┐
           │ Pi (CAN) │
           └──────────┘
                 │
         ┌───────┼───────┬───────┐
         │       │       │       │
        CAN     CAN     CAN     CAN
         │       │       │       │
       ┌─▼─┐   ┌─▼─┐   ┌─▼─┐   ┌─▼─┐
       │ M1│   │ M2│   │ M3│   │ M4│
       └────┘   └────┘   └────┘   └────┘
```

### Daisy Chain (Acceptable)
```
Pi → M1 → M2 → M3 → M4
```

## Network Protocols and Ports

### CAN Bus
- **Standard**: ISO 11898-1
- **Speed**: 500 kbps or 1 Mbps (configured in firmware)
- **Frame Format**: Standard (11-bit ID) or Extended (29-bit ID)
- **Isolation**: Galvanically isolated from other networks
- **Max Distance**: 40m at 500 kbps, 10m at 1 Mbps

### Docker Network (Internal)
```
Container A ←→ MQTT Broker (localhost:1883)
Container B ←→ MQTT Broker
Container C ←→ REST API (localhost:3000)
```

### MQTT (Internal Message Bus)
```
Protocol: MQTT 3.1.1 / 5.0
Broker: Mosquitto (in Docker)
Port: 1883 (internal), 8883 (encrypted, if used)
Topics: Hierarchical (e.g., tc/gps/position, tc/power/voltage)
QoS: 0 (fire-and-forget) or 1 (at-least-once)
```

### REST API (Cloud Communication)
```
Protocol: HTTPS
Port: 443 (to cloud), 3000 (local development)
Authentication: API Key / JWT Token
Data Format: JSON
```

### WebSocket (Real-time Updates)
```
Protocol: WSS (WebSocket Secure) / WS
Port: 443 (HTTPS upgrade), 8080 (local dev)
Purpose: Real-time device updates to UI
```

### Bluetooth (Optional Direct Communication)
```
Standard: Bluetooth 4.0 LE or Classic
Range: 10-100m (depending on module)
Purpose: Direct mobile app ↔ module communication
```

## Time Synchronization

> **Headwaters is the canonical LAN NTP server for the rig.** Every Linux-class compute device on the platform — Peregrine, Playbill, and any future compute module — syncs to it. This is deliberate, not incidental.

### Why this matters

When scheduling features come online (timed automations, schedule-based light / relay / thermostat / Playbill actions), every device on the rig needs to evaluate triggers against the **same clock** — and that clock has to work without internet.

Putting Headwaters in charge of time:

- Guarantees every Linux-class device on the rig agrees on what "08:00 Tuesday" means, even on a multi-day off-grid trip.
- Removes any dependence on external NTP pools as a hard requirement. Upstream pools are still used opportunistically when reachable, but the system does not stop working when they aren't.
- Implements **Cloud Optional** at the time layer — scheduling is a vehicle-local capability, not a cloud-dependent one.

### How it works

```
                 ┌────────────────────────────────────────┐
                 │  Headwaters (NTP server, port 123/UDP) │
                 │  ───────────────────────────────────── │
                 │  chronyd:                              │
                 │   ├─ upstream pool when reachable      │
                 │   │  (Debian NTP pool, iburst)         │
                 │   ├─ time-from-bearing.service feeds   │
                 │   │  GNSS UTC into the system clock    │
                 │   │  via Bearing's GpsDateTime CAN     │
                 │   │  frame (0x06) when offline         │
                 │   └─ local stratum 10 fallback so LAN  │
                 │      clients don't get "unsynced"      │
                 │                                        │
                 │  Allows: 192.168/16, 10/8, 172.16/12   │
                 └────────────────────┬───────────────────┘
                                      │  NTP / 123 UDP
                  ┌───────────────────┼──────────────────┐
                  │                   │                  │
                  ▼                   ▼                  ▼
            ┌──────────┐        ┌──────────┐       ┌──────────┐
            │Peregrine │        │ Playbill │       │ (future  │
            │ (Q6A)    │        │  (Q6A)   │       │  Linux   │
            └──────────┘        └──────────┘       │  device) │
                                                    └──────────┘
```

ESP32-class MCU modules do **not** run NTP and are out of scope for this section. When ordering matters between MCU events, the consumer is responsible for timestamping at receive time on a Linux-class device that does have synced time, or for using one of the time-bearing CAN messages (`GpsDateTime` `0x06` from Bearing).

### Configuration pointers

- Headwaters `chronyd` config: `/Product/TrailCurrentHeadwaters/config/chrony/chrony.conf`
- GNSS-fed time service: `/Product/TrailCurrentHeadwaters/local_code/time-from-bearing.{py,service}`
- Peregrine NTP client: pointed at `headwaters.local` during image build
- Playbill NTP client: pointed at `headwaters.local` during image build

### Reachable from anywhere on the rig

The chrony config allows queries from `192.168.0.0/16`, `10.0.0.0/8`, and `172.16.0.0/12` so the rig network can use any RFC 1918 range. A `local stratum 10` clause lets Headwaters serve from its own clock when no upstream pool is reachable — clients still get a synced time source, they just won't believe it more than a real internet source if one shows up.

## IP Address Planning

### Docker Container IPs (Gateway)
```
Network: 172.20.0.0/16 (example)
- MQTT Broker:     172.20.0.2:1883
- REST API:        172.20.0.3:3000
- Web UI:          172.20.0.4:80
- Database:        172.20.0.5:5432
```

### In-Vehicle Compute Device IPs
```
Device Hostname: trailcurrent-vehicle (or specific name)
Device IP (Ethernet): 192.168.1.100 (example)
Device IP (WiFi): 192.168.1.101 (if applicable)
```

### Cloud Service IPs
```
Cloud API: https://api.trailcurrent.local (local)
           https://api.trailcurrent.cloud (production)
```

## Firewall Rules

### In-Vehicle Compute Device (Ingress)
```
Allow:
- Port 22 (SSH) from admin subnet
- Port 80/443 (HTTP/HTTPS) from local network
- Port 123 (NTP/UDP) from rig LAN (Headwaters serves NTP to all
  Linux-class devices on the rig — see Time Synchronization above)
- Port 1883 (MQTT) from localhost only
- Port 8883 (MQTTS) from rig LAN (for in-vehicle clients) and
  outbound to the cloud broker

Deny:
- All other ports
- All external access except specified cloud endpoints
```

### Cloud Server (Ingress)
```
Allow:
- Port 443 (HTTPS) from internet
- Port 22 (SSH) from admin subnet

Deny:
- Direct MQTT access from internet
- Direct database access from internet
```

### CAN Bus
```
Isolated network:
- No routing to external networks
- Only in-vehicle compute CAN transceiver accesses it
```

## Security Considerations

1. **CAN Bus Isolation**: No direct internet access
2. **Docker Network**: Internal MQTT not exposed
3. **Cloud Authentication**: All cloud API calls require credentials
4. **Encryption**: HTTPS for cloud communication
5. **Firewall**: Strict ingress/egress rules on Pi
6. **VPN (Optional)**: For remote admin access

## Redundancy & Failover

```
Primary Path (Connected):
Device → CAN → Pi → MQTT → Cloud

Fallback Path (Cloud Down):
Device → CAN → Pi → Local Storage

Recovery (Cloud Returns):
Pi syncs cached data with cloud
```

## Bandwidth Considerations

### CAN Bus
- ~1-10 Mbps depending on message frequency
- Typically 20-50% utilization in normal operation

### MQTT
- ~10-100 kbps typical
- Spikes during bulk data transfer

### Cloud (Internet)
- ~1-10 Mbps bandwidth required
- Depends on update frequency and data volume

---

See also:
- [SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md) - Component details
- [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) - CAN configuration
- [10_Reference/MQTT_TOPICS.md](../10_Reference/MQTT_TOPICS.md) - MQTT topic structure
