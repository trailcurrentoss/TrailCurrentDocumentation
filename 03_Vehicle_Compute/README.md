# TrailCurrent In-Vehicle Compute System

Complete guide to the edge computing system that serves as the gateway between hardware modules and the cloud.

## Overview

The In-Vehicle Compute system — branded **TrailCurrent Headwaters** — is an edge computing device running on a Raspberry Pi Compute Module 5 (CM5) on a standard carrier board with a Waveshare RS485 CAN HAT (B). The CM5 + carrier board replaces the earlier Pi 5 + NVME Base + custom CAN HAT stack: it is more compact, more readily available, cheaper, and can be fully assembled from off-the-shelf parts with no soldering.

It runs containerized services that:
- Convert CAN bus messages to MQTT for local wireless communication (when internet unavailable) and cloud sync (when connected to the cloud application, Farwatch)
- Cache data locally for offline operation and autonomous vehicle control
- Distribute OTA firmware updates to hardware modules (over CAN for MCU modules; over mDNS + HTTP for wireless modules)
- Provide a local REST + WebSocket API for vehicle status queries
- Route commands from users/cloud to hardware modules via CAN
- **Serve as the canonical LAN NTP server** for the rig — every Linux-class device on the platform (Peregrine, Playbill, future compute modules) syncs to Headwaters so all scheduling and time-ordered logic agrees on the clock, with or without internet (see [Time Synchronization](#time-synchronization) below)

> **Lite variant:** [TrailCurrent Baseflow](../README.md#baseflow-lite-edge-gateway) is a scaled-down variant of Headwaters that runs on the Arduino Uno Q (4GB) and provides the same core frontend/backend/MQTT/CAN stack without tile server, cloud sync, OTA distribution, or Node-RED. Use it when all you need is a self-contained edge gateway.

> **Alternate hardware platform (work in progress):** A **Radxa Dragon Q6A variant** of Headwaters lives under [`/Product/TrailCurrentHeadwaters/RADXAQ6A/`](../../TrailCurrentHeadwaters/RADXAQ6A/) and builds a flashable Q6A image with the same Docker/MQTT/MongoDB/tileserver/backend/frontend/CAN bridge stack. **It is not recommended for shipping use yet** — further hardware and field testing is required. The CM5 build remains the canonical, recommended platform for any real deployment. The Q6A variant is fully self-contained (no shared scripts with the CM5 pipeline) so the two can evolve independently.

## Architecture

```
┌─────────────────────────────────────┐
│    In-Vehicle Compute (Edge)        │
│  (Raspberry Pi CM5 + CAN HAT)       │
├─────────────────────────────────────┤
│                                     │
│  ┌────────────────────────────────┐ │
│  │  Docker Container Services     │ │
│  ├────────────────────────────────┤ │
│  │ • Backend (CAN↔MQTT, REST API) │ │
│  │ • MQTT Broker (Mosquitto)      │ │
│  │ • MongoDB (local data store)   │ │
│  │ • Frontend (local dashboard)   │ │
│  │ • Tile Server (offline maps)   │ │
│  └────────────────────────────────┘ │
│                 ↑                    │
│         ┌───────┴────────┐          │
│         │                │          │
│   Ethernet/WiFi      CAN Bus        │
│    (To Cloud)    (To Modules)      │
│         │                │          │
└─────────────────────────────────────┘
```

## Key Components

### 1. Backend (CAN↔MQTT Gateway + REST API)
Node.js service that converts CAN messages to MQTT messages for internal routing and cloud sync, exposes REST API endpoints for status queries and commands, and manages device configuration and CAN ID mappings.

### 2. Mosquitto MQTT Broker
Message broker for internal container communication and cloud connectivity.

### 3. MongoDB
Local document store for sensor data, command history, device configuration, and system events.

### 4. Frontend
Local web dashboard for vehicle status, diagnostics, and configuration.

### 5. Tile Server
Offline vector tile server for maps that work without internet connectivity.

### 6. NTP Server (chronyd)
Headwaters serves NTP to the rig LAN so every Linux-class device on the platform (Peregrine, Playbill, future compute modules) syncs to the same clock. See [Time Synchronization](#time-synchronization) below for the full rationale.

## Time Synchronization

Headwaters is the **canonical LAN NTP server** for the rig. It serves on UDP port 123 and allows queries from `192.168.0.0/16`, `10.0.0.0/8`, and `172.16.0.0/12` so any RFC 1918 rig network works without reconfiguration.

This is foundational, not incidental. When **scheduling features** come online (timed automations, schedule-based light / relay / thermostat / Playbill actions), every device on the rig needs to evaluate triggers against the **same clock** — and that clock has to work without internet. Centralizing the time source on Headwaters makes that possible.

### How the time source resolves

`chronyd` on Headwaters draws from three sources, in order:

1. **Upstream NTP pool** (Debian NTP pool, iburst, max 4 sources) — used when internet is reachable.
2. **GNSS UTC from Bearing** — `time-from-bearing.service` reads Bearing's `GpsDateTime` CAN frame (`0x06`) and feeds the UTC time into the system clock. This is the offline fallback: a rig rolling for days without internet still gets correct time, because Bearing's GNSS receiver always has it.
3. **Local stratum 10 fallback** — if neither upstream nor GNSS is available (e.g., a stationary rig with no internet and no satellite lock), chronyd serves from its own clock so LAN clients don't get "unsynchronised" errors. Any real internet source beats local stratum 10 if one shows up.

`makestep 1.0 3` is configured so the first 3 updates after startup take an immediate jump (instead of slewing) when the offset is > 1 s, which matches the boot-with-stale-RTC case.

### MCU modules are out of scope

ESP32-class hardware modules do not run NTP. When ordering matters between MCU events, the consumer is responsible for timestamping on Headwaters (or another Linux-class device) at receive time. Bearing's `GpsDateTime` (`0x06`) is also available on the bus for any module that wants a direct time reference without going through Headwaters.

### Configuration pointers

- `chronyd` config: `/Product/TrailCurrentHeadwaters/config/chrony/chrony.conf`
- GNSS-fed time service: `/Product/TrailCurrentHeadwaters/local_code/time-from-bearing.{py,service}`

See [01_Architecture/NETWORK_TOPOLOGY.md — Time Synchronization](../01_Architecture/NETWORK_TOPOLOGY.md#time-synchronization) for the architectural diagram and rationale.

## Setup & Installation

### Prerequisites
- Raspberry Pi Compute Module 5 (CM5) on a standard carrier board
- Waveshare RS485 CAN HAT (B)
- MicroSD card or eMMC (32GB+)
- Power supply (adequate for CM5 and CAN HAT)
- Ethernet or WiFi connectivity

### Installation Steps

1. [Install Raspberry Pi OS](SETUP_GUIDE.md#step-1-install-raspberry-pi-os)
2. [Configure CAN Interface](SETUP_GUIDE.md#step-2-configure-can-interface)
3. [Install Docker and Compose](SETUP_GUIDE.md#step-3-install-docker)
4. [Deploy Services](SETUP_GUIDE.md#step-4-deploy-services)
5. [Configure Containers](Configuration/CONFIG_FILES.md)
6. [Test & Validate](SETUP_GUIDE.md#step-5-test-and-validate)

See [SETUP_GUIDE.md](SETUP_GUIDE.md) for detailed instructions.

## Configuration

### CAN Interface Configuration
Located in `/etc/network/interfaces.d/can0`

### Docker Services
Defined in `/home/pi/trailcurrent/docker-compose.yml`

### Application Configuration
Located in `/etc/trailcurrent/config/`

See [Configuration/CONFIG_FILES.md](Configuration/CONFIG_FILES.md) for details.

## Containerized Services

### Docker-Compose Structure

```yaml
services:
  backend:          # CAN ↔ MQTT bridge + REST API
  frontend:         # Local web dashboard
  mosquitto:        # MQTT broker
  mongodb:          # Local document store
  tileserver:       # Offline vector map tiles
```

Each service runs independently and communicates via the Docker network.

## Gateway Functionality

The in-vehicle compute gateway performs bidirectional conversion between the CAN bus (wired vehicle network) and MQTT (wireless local network). This enables:
- **Local wireless communication** between services when internet is unavailable
- **Data synchronization** with cloud when internet becomes available
- **Autonomous operation** of vehicle systems regardless of WAN connectivity
- **Decoupling** of hardware modules from cloud dependency

### CAN to MQTT Conversion

```
CAN Frame (from hardware)
  ├─ Frame ID
  ├─ Data bytes [0-7]
  └─ Timestamp
       ↓ Parser
  ├─ Device type (GPS, Temp, Power, etc.)
  ├─ Message type (status, command, etc.)
  └─ Payload
       ↓ Mapper
  MQTT Topic: tc/[device]/[message_type]
  Payload: JSON with parsed data
```

### MQTT to CAN Conversion

```
MQTT Message (from cloud or UI)
  ├─ Topic: tc/command/[device]/[action]
  ├─ Payload: {"action": "...", "params": {...}}
  └─ Timestamp
       ↓ Validator
  ├─ Check permissions
  ├─ Validate parameters
  └─ Check device state
       ↓ Converter
  CAN Frame:
  ├─ Frame ID: (device-specific)
  ├─ Data bytes: [command, params...]
  └─ Priority
       ↓ Sender
  Transmitted on CAN bus
```

## Data Storage

### Local MongoDB
Stores:
- Last 24-48 hours of sensor readings
- Command history
- Device configuration
- System events

### File System Cache
Stores:
- Firmware binaries (for OTA distribution)
- Configuration files
- Log files (with rotation)

## Network Connectivity

### Internet Connection
- Ethernet (primary, recommended)
- WiFi (secondary, optional)
- Automatic failover if configured

### Local Operation (Offline Mode)
When internet is unavailable, the in-vehicle compute system continues to operate autonomously:
- Local MQTT broker allows all vehicle systems to communicate
- Vehicle can accept commands via local wireless access
- Data is cached locally for later sync
- Vehicle remains fully functional for critical operations

### Cloud Sync
- Automatic connection to cloud MQTT broker (when internet available)
- Offline queue preserves commands/data during disconnection
- Auto-sync of cached data when connection restored
- Seamless operation transition between offline and online modes

## OTA Firmware Updates

The Vehicle Pi distributes firmware updates to hardware modules:

1. Cloud sends update notice via MQTT
2. Pi downloads new firmware binary
3. Pi queues update command with CAN priority
4. Target module receives and updates
5. Module confirms completion
6. Pi logs successful update

See [Deployment/PI_DEPLOYMENT.md](Deployment/PI_DEPLOYMENT.md) for update procedures.

## Monitoring & Status

### System Health Checks
- Container status (running/stopped)
- CAN bus connectivity
- Cloud connection status
- Disk space and memory usage
- Database integrity

### Logging
All services log to Docker stdout (accessed via `docker logs`)

Persistent logs in `/var/lib/trailcurrent/logs/`

## Security

### Local Network
- Firewall configured to deny external access
- SSH restricted to local network
- MQTT broker internal only (no external access)

### Cloud Communication
- MQTT over TLS (if configured)
- API authentication tokens
- Encrypted credentials in NVS

## Performance Considerations

### CAN Bus Load
- Gateway processes all CAN messages
- Typical load: 5-20% bus utilization
- MQTT publishing rate: 1-50 messages/second

### Memory Usage
- Docker services: ~300-500MB total (MongoDB is the heaviest)
- MongoDB working set: ~100-200MB
- Free RAM: typically 1GB+ on an 8GB CM5

### Disk Usage
- System: ~2-3GB
- MongoDB database: ~1-5GB (with 24-48 hours data)
- Tile server map data: size depends on region(s)
- Log files: ~500MB-1GB (with rotation)
- Free space: ~15GB+ recommended

## Maintenance

### Regular Tasks
- Monitor system logs
- Check disk space
- Update firmware
- Backup configuration

### Troubleshooting
See [09_Troubleshooting/](../09_Troubleshooting/) for common issues.

## Advanced Topics

### Custom Containers
You can add custom Docker services for:
- Additional processing
- Local analytics
- Custom integrations
- Extended functionality

### Direct Access
For administrative access:
```bash
ssh pi@[pi-ip-address]
docker ps                    # See running containers
docker logs [container-name] # View logs
docker exec -it [container-name] /bin/bash  # Enter container
```

## Documentation

- [SETUP_GUIDE.md](SETUP_GUIDE.md) - Step-by-step installation
- [CAN_MQTT_GATEWAY.md](CAN_MQTT_GATEWAY.md) - Gateway details
- [DOCKER_CONTAINERS.md](DOCKER_CONTAINERS.md) - Container specifications
- [Configuration/CONFIG_FILES.md](Configuration/CONFIG_FILES.md) - Configuration options
- [Deployment/PI_DEPLOYMENT.md](Deployment/PI_DEPLOYMENT.md) - Deployment procedures
- [SMS_NOTIFICATIONS.md](SMS_NOTIFICATIONS.md) - SMS alarm notifications via GL-iNet router

## Source Code

Vehicle Compute system source: `/Product/TrailCurrentHeadwaters/`

---

See also:
- [01_Architecture/](../01_Architecture/) - System architecture
- [08_Deployment/](../08_Deployment/) - Deployment procedures
- [09_Troubleshooting/](../09_Troubleshooting/) - Common issues
