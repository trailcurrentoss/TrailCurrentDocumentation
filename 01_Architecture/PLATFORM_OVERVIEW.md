# TrailCurrent Platform Overview

## What is TrailCurrent?

**TrailCurrent is an Open Source Software Defined Vehicle (SDV) Platform** - a complete architecture for vehicle monitoring, control, and management. Unlike traditional IoT platforms designed for generic applications, TrailCurrent is specifically engineered for vehicles with:

- **Safety-first architecture** - All critical vehicle functions have hardwired, non-networked backup controls
- **Complete vehicle autonomy** - Vehicles operate with full functionality without internet or cloud
- **Software-defined control** - Programmable vehicle behavior through open-source software running on in-vehicle compute
- **Data sovereignty** - All vehicle data stays under user control, no vendor lock-in

This makes it fundamentally different from IoT platforms - it's a purpose-built vehicle control platform with safety, autonomy, and openness at its core.

---

## System Layers

A comprehensive overview of the TrailCurrent platform architecture and how its components work together.

## System Layers

The TrailCurrent platform is organized into three layers, where the **Device and Edge layers are essential** and the **Cloud layer is completely optional**:

```
┌─────────────────────────────────────────────┐
│   CLOUD LAYER (OPTIONAL)                    │
│   (Your Private Cloud - Self-Hosted)        │
│   (Analytics, Remote Control, Web UI)       │
│   (Can be skipped entirely - not required)  │
└─────────────────────┬───────────────────────┘
                      │ Internet/WiFi (optional)
┌─────────────────────▼───────────────────────┐
│   EDGE LAYER (In-Vehicle Compute)           │
│   ⭐ ESSENTIAL - Operates Fully Standalone  │
│   (Linux SBC - Gateway, Processing)         │
│   (Works with or without cloud)             │
└─────────────────────┬───────────────────────┘
                      │ CAN Bus
┌─────────────────────▼───────────────────────┐
│   DEVICE LAYER (Hardware Modules)           │
│   ⭐ ESSENTIAL - Core Vehicle Systems       │
│   (ESP32 Sensors, Controllers, Gateways)    │
└─────────────────────────────────────────────┘
```

## Layer Details

### Device Layer (Hardware Modules)

**Role**: Sensing, control, and communication

**Components**:
- **Sensor Modules**: GPS, Temperature, Air Quality
- **Control Modules**: Power Control, Heater, Vehicle Leveler
- **Gateway Modules**: CAN/EspNow Gateway, BT Gateway, Shunt Gateway
- **Interface Modules**: Wall displays, remote controls, button panels

**Communication**: CAN bus (primary), EspNow (secondary), Bluetooth

**Technology**: ESP32 microcontrollers, ESP-IDF framework

### Edge Layer (In-Vehicle Compute)

**Role**: Edge processing, gateway functionality, local intelligence

**Components**:
- CAN-to-MQTT gateway
- Docker container orchestration
- Local configuration and management
- OTA firmware update distribution
- Local data logging and caching

**Compute Devices**: Raspberry Pi 4/5, Orange Pi, Jetson Nano, or similar Linux-based single-board computer

**Communication**: CAN bus (to devices), Ethernet/WiFi (to cloud), MQTT (internal messaging)

**Technology**: Linux, Docker, Node.js, Python

### Cloud Layer (Optional)

⚠️ **IMPORTANT: This layer is completely optional.** Vehicles operate with full autonomy without it.

**Role**: Optional centralized monitoring, analytics, remote control, and user interface

**Key Principles**:
- **Your Private Cloud** - Deploy wherever you want: on your own servers, VPS, home NAS, etc.
- **Complete Data Privacy** - All vehicle data stays under your control, never sent to third parties
- **No External Dependencies** - No Big Tech APIs, no vendor lock-in, no tracking
- **Fully Self-Hosted** - You run and control your own cloud infrastructure

**Components** (if deployed):
- REST API backend (Node.js/Express)
- PostgreSQL/SQLite database
- Web frontend (HTML/CSS/JavaScript)
- MQTT message broker
- Map and analytics engine

**Communication**: HTTPS (to users), MQTT (to in-vehicle compute), WebSocket (real-time updates)

**Technology**: Node.js, Express, HTML/CSS/JS, Docker - **100% Open Source**

## Communication Architecture

```
Hardware Modules
    ├─ CAN Bus → In-Vehicle Compute
    ├─ BT/WiFi → Cloud
    └─ EspNow → Direct Module-to-Module
                    ↓
        In-Vehicle Compute Gateway
                    ├─ Converts CAN → MQTT
                    ├─ Local processing
                    └─ Local storage
                         ↓
                    Cloud Application
                    ├─ REST API
                    ├─ Web Dashboard
                    ├─ Mobile Apps
                    └─ Analytics/Reporting
```

## Hardware Module Categories

### Sensors
- GPS Module - Vehicle location tracking
- Temperature Sensor - Environmental monitoring
- Air Quality Module - Pollution/particulate tracking
- Cabinet & Door Sensor - Intrusion detection
- Shunt Gateway - Power monitoring

### Control Systems
- Power Control Module (PCM) - Main power distribution
- Electric Heater Control - Heater management
- Vehicle Leveler - Auto-leveling control
- MPPT CAN Gateway - Solar charge controller

### Communication & External Systems
- BT Gateway - Bluetooth connectivity
- CAN/EspNow Gateway - Multi-protocol gateway
- External Systems Monitor - Interface with external subsystems via standard connectors

### User Interfaces
- 8-Button Panel - Physical control
- Wall-Mounted Displays - Status display (7", standard)
- EspNow Remote Control - Wireless remote
- Waveshare Remote - Specific remote model

## Data Flow

1. **Hardware Sensors** collect data
2. **Hardware Modules** send data via CAN bus
3. **In-Vehicle Compute Gateway** receives CAN messages
4. **Gateway** converts to MQTT and forwards to Cloud
5. **Cloud Application** processes and stores data
6. **Web/Mobile UI** displays to users
7. **Users** can send commands back through the chain

## Key Technologies

| Layer | Technology | Purpose |
|-------|-----------|---------|
| Hardware | ESP32, C/C++ | Embedded firmware |
| Hardware | CAN Bus | Reliable device communication |
| Edge | Linux SBC | Edge computing device |
| Edge | Docker | Container orchestration |
| Edge | MQTT | Internal messaging |
| Cloud | Node.js/Express | Backend API |
| Cloud | PostgreSQL | Data storage |
| Cloud | HTML/CSS/JS | Frontend UI |
| Cloud | WebSocket | Real-time updates |

## Fundamental Principles

These principles are **core to TrailCurrent's mission** and must be preserved in all development:

### 1. Cloud is Optional
- The vehicle operates with **complete autonomy** without any cloud connection
- Cloud is never required - it's purely optional for enhanced monitoring/management
- If you choose to use cloud, it's **your private cloud**, self-hosted wherever you want

### 2. Data Privacy First
- **All vehicle data stays private** - never sent to third parties
- No external API dependencies (no AWS, Google, Azure, etc. required)
- **No vendor lock-in** - you control your entire system
- **No tracking** - complete data sovereignty

### 3. 100% Open Source
- **Everything is open source**: firmware, CAD files, EDA schematics, backend code, frontend code, build tools, documentation
- No proprietary components or black-box services
- Community-driven development with transparent governance

## Design Principles

1. **Modular**: Each module is independent and can be updated/replaced
2. **Resilient**: Works locally even if cloud connection is lost - vehicles operate fully autonomously with local MQTT when offline, then sync when reconnected
3. **Secure**: Authenticated communication, encrypted credentials, no external service dependencies
4. **Scalable**: Supports multiple vehicles/locations
5. **Real-Time**: Low-latency critical control updates via local CAN/MQTT
6. **Open**: Uses standard protocols (CAN, MQTT, REST, WebSocket) - all 100% open source

## System Boundaries

- **Device Boundary**: ESP32 ↔ CAN Bus (hardware interface)
- **Edge Boundary**: CAN Bus ↔ In-Vehicle Compute (gateway function)
- **Cloud Boundary**: In-Vehicle Compute ↔ Internet (firewall/gateway)
- **User Boundary**: Cloud ↔ Browser/Mobile (web interfaces)

---

See also:
- [TrailCurrent Architecture Overview](https://trailcurrent.com/architecture.html) - Visual overview of the platform architecture on the TrailCurrent website
- [SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md) - Detailed component interactions
- [NETWORK_TOPOLOGY.md](NETWORK_TOPOLOGY.md) - Network and communication setup
- [DATA_FLOW.md](DATA_FLOW.md) - Detailed data movement through system
- [02_Hardware_Modules/](../02_Hardware_Modules/) - Individual module documentation
