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
│   (Raspberry Pi CM5 - Gateway, Processing) │
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
- **Sensor Modules**: Bearing (GNSS), Borealis (environment), Picket (doors), Ampline (power), Plateau (level)
- **Control Modules**: Torrent (power delivery), Therma (climate), Solstice (solar)
- **Gateway Modules**: Aftline (trailer monitor), RV-C Gateway (coming soon)
- **Interface Modules**: Tapper (buttons), Fireside (wireless display), Milepost (hardwired display), Spotter (trailer monitor display)
- **Voice & AI**: Peregrine (voice assistant)

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

**Compute Device**: Raspberry Pi Compute Module 5 (CM5) on a standard carrier board with Waveshare RS485 CAN HAT (B) — fully off-the-shelf, no custom PCBs.

**Communication**: CAN bus (to devices), Ethernet/WiFi (to cloud), MQTT (internal messaging)

**Technology**: Linux (Raspberry Pi OS), Docker, Node.js

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
- Bearing - GNSS location, heading, altitude, and precise timing
- Borealis - Temperature, humidity, CO2, indoor air quality
- Picket - Cabinet and door open/closed status monitoring
- Ampline - Power consumption and state-of-charge via Victron Shunt
- Plateau - Tilt/level measurement on both axes

### Control Systems
- Torrent - 8-channel smart power delivery with switching and PWM dimming
- Therma - Dual-relay automatic heating/cooling
- Solstice - Victron MPPT solar charge controller interface

### Communication & External Systems
- Aftline - Trailer wiring harness monitor (all 7 pins)
- RV-C Gateway *(Coming Soon)* - RV-C protocol gateway

### User Interfaces
- Tapper - Physical 8-button interface for Torrent commands
- Fireside - Wireless battery-powered touchscreen display with wall cradle
- Milepost - Hardwired CAN bus touchscreen (always-on)
- Spotter - In-vehicle display for monitoring trailer while towing

### Voice & AI
- Peregrine - AI voice assistant with hands-free system control

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
| Edge | Raspberry Pi CM5 + Waveshare CAN HAT (B) | Edge computing device |
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
