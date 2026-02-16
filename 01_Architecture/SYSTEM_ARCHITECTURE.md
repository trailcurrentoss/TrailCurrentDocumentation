# TrailCurrent System Architecture

## Software Defined Vehicle Architecture

TrailCurrent implements a **Software Defined Vehicle (SDV) architecture** where vehicle behavior, control, and monitoring are defined through open-source software running on in-vehicle compute, rather than through proprietary hardware or cloud services.

**Architecture Principles:**
- **Vehicle Autonomy** - Complete functionality without cloud or internet
- **Safety-First** - Critical functions always have wired, non-networked control
- **Open Standards** - CAN bus for hardware, MQTT for messaging, open protocols throughout
- **Edge-Centric** - In-vehicle compute is the intelligence layer, cloud is optional enhancement
- **User Control** - No vendor lock-in, complete data sovereignty

---

## Comprehensive System Diagram

```
┌────────────────────────────────────────────────────────────────┐
│                     CLOUD TIER                                 │
├────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐         │
│  │  Web UI      │  │  Mobile App  │  │  Analytics   │         │
│  │ (Browser)    │  │  (Android)   │  │  (Reports)   │         │
│  └──────┬───────┘  └──────┬───────┘  └──────┬───────┘         │
│         │                 │                 │                  │
│         └─────────────────┼─────────────────┘                  │
│                           │ HTTPS/WebSocket                    │
│                    ┌──────▼───────┐                            │
│                    │  REST API    │                            │
│                    │  (Express)   │                            │
│                    └──────┬───────┘                            │
│         ┌──────────┬──────┼──────────┬──────────┐             │
│         │          │      │          │          │             │
│  ┌──────▼──┐  ┌────▼──┐  │  ┌──────▼──┐  ┌────▼──┐           │
│  │PostgreSQL│  │Redis │  │  │ MQTT    │  │ File  │           │
│  │ Database │  │Cache │  │  │ Broker  │  │Store  │           │
│  └──────────┘  └──────┘  │  └────┬────┘  └───────┘           │
│                          │       │                            │
└──────────────────────────┼───────┼────────────────────────────┘
                           │       │
              ═════════════╬═══════╬═════════════
                     Internet/WiFi (HTTPS/MQTT)
              ═════════════╬═══════╬═════════════
                           │       │
└──────────────────────────┼───────┼────────────────────────────┘
│              EDGE TIER (In-Vehicle Compute)                   │
│        (Raspberry Pi, Orange Pi, Jetson Nano, etc.)          │
├────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌────────────────────────────────────────────────────┐       │
│  │         Docker Container Orchestration             │       │
│  │                                                    │       │
│  │  ┌─────────────┐  ┌─────────────┐  ┌──────────┐  │       │
│  │  │CAN-to-MQTT  │  │ Mosquitto   │  │ App      │  │       │
│  │  │ Gateway     │  │ (MQTT Broker)  │ Config   │  │       │
│  │  └────┬────────┘  └──────┬──────┘  └──────────┘  │       │
│  │       │                  │                       │       │
│  │  ┌────▼──────────────────▼────────┐              │       │
│  │  │ Local Data Storage & Logging    │              │       │
│  │  │ (SQLite, File System)           │              │       │
│  │  └──────────────────────────────────┘              │       │
│  │                                                    │       │
│  └────────────────────────────────────────────────────┘       │
│                        ▲                                       │
│                        │ CAN Bus                               │
│                        │ (Isolated Network)                    │
│                        ▼                                       │
│  ┌────────────────────────────────────┐                       │
│  │     CAN Bus Transceiver Module      │                       │
│  └────────────────────────────────────┘                       │
│                                                                 │
└────────────────────────────────────────────────────────────────┘
                           │
              ═════════════╬═════════════
                      CAN Bus (Isolated)
              ═════════════╬═════════════
                           │
┌────────────────────────────────────────────────────────────────┐
│              DEVICE TIER (ESP32 Modules)                       │
├────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐        │
│  │   Sensors    │  │  Controllers │  │   Interfaces │        │
│  ├──────────────┤  ├──────────────┤  ├──────────────┤        │
│  │ GPS Module   │  │ Power Control│  │ 8-Button     │        │
│  │ Temp Sensor  │  │ Heater Ctrl  │  │ Panel        │        │
│  │ Air Quality  │  │ Leveler      │  │ Wall Display │        │
│  │ Door Sensor  │  │ MPPT Gateway │  │ Remote Ctrl  │        │
│  │ Shunt        │  └──────────────┘  │ Waveshare    │        │
│  └──────┬───────┘                     │ Remote       │        │
│         │                              └──────┬───────┘        │
│         └──────────┬────────────────────────┬─┘               │
│                    │ CAN Messages           │ CAN Messages    │
│                    ▼                        ▼                 │
│            ┌──────────────────────┐                           │
│            │  CAN Bus Line        │                           │
│            │ (All modules share)   │                           │
│            └──────────────────────┘                           │
│                    │                                          │
│            ┌───────┴──────┬────────────┐                      │
│            │              │            │                      │
│  ┌─────────▼────┐  ┌──────▼──────┐  ┌─▼───────────┐         │
│  │ CAN/EspNow   │  │   BT        │  │  Secondary  │         │
│  │ Gateway      │  │   Gateway   │  │  Gateways   │         │
│  │ (to Pi)      │  │ (Optional)  │  │ (Optional)  │         │
│  └──────────────┘  └─────────────┘  └─────────────┘         │
│                                                                 │
└────────────────────────────────────────────────────────────────┘
```

## Component Interactions

### CAN Bus Communication
```
Module A ──┐
Module B ──┤→ CAN Bus ──→ CAN Transceiver ──→ In-Vehicle Compute
Module C ──┤                                   (Gateway)
Module D ──┘
```

All modules share a single CAN bus line. Each module has a unique CAN ID.

### MQTT Message Flow
```
CAN Message
    ↓
[CAN-to-MQTT Gateway]
    ↓
MQTT Topic (e.g., "tc/gps/position")
    ↓
├→ Local Subscribers (Containers)
├→ Mosquitto Broker
    └→ Cloud Application (if connected)
```

### REST API Communication
```
Web UI / Mobile App
    ↓ HTTPS Request
[REST API Server]
    ↓
├→ Query/Update Database
├→ Publish MQTT (if device control needed)
├→ Query Local Data Cache
    ↓ HTTPS Response
Return JSON Data
```

## Network Isolation

```
Internet (Public)
    ↓
[Pi Firewall/NAT]
    ↓
├─ Docker Network (Private)
│  └─ Containers communicate via MQTT
│
└─ CAN Bus (Isolated)
   └─ Hardware modules only
   └─ No external access
```

## Module Responsibilities

### Hardware Modules (Device Tier)
- Collect sensor data
- Execute control commands
- Maintain CAN bus communication
- Handle local state and logic
- Report status regularly

### In-Vehicle Compute Gateway (Edge Tier)
- Translate CAN ↔ MQTT
- Cache local data
- Route messages to cloud
- Handle OTA firmware updates
- Provide local UI/API (optional)

### Cloud Application (Cloud Tier)
- Store historical data
- Provide analytics
- Serve web/mobile interfaces
- Support remote control
- Generate reports

## Communication Protocols

| Link | Protocol | Direction | Format |
|------|----------|-----------|--------|
| Device ↔ Device | CAN Bus | Bi-directional | CAN Frames |
| Device → Pi | CAN Bus | One-way | CAN Frames |
| Pi ↔ Pi Services | MQTT | Pub/Sub | JSON |
| Pi → Cloud | MQTT over TCP | One-way (primarily) | JSON |
| User ↔ Cloud | HTTPS | Bi-directional | JSON |
| User ↔ Cloud | WebSocket | Bi-directional | JSON |

## System States

### Disconnected State
- Devices operate independently on CAN bus
- Pi caches data locally
- Cloud cannot command devices
- Data syncs when reconnected

### Connected State
- Full bi-directional communication
- Real-time updates to cloud
- Remote commands execute immediately
- Seamless redundancy between local and cloud

## Scalability Considerations

- Multiple vehicles: Each Pi is independent
- Multiple cloud instances: Load-balanced API servers
- Data storage: Scalable database (PostgreSQL)
- Real-time updates: Persistent MQTT connections

---

See also:
- [TrailCurrent Architecture Overview](https://trailcurrent.com/architecture.html) - Visual overview on the TrailCurrent website
- [NETWORK_TOPOLOGY.md](NETWORK_TOPOLOGY.md) - Network configuration details
- [DATA_FLOW.md](DATA_FLOW.md) - Message flow examples
- [DEPLOYMENT_ARCHITECTURE.md](DEPLOYMENT_ARCHITECTURE.md) - Deployment topology
