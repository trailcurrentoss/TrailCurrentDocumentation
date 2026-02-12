# TrailCurrent In-Vehicle Compute System

Complete guide to the edge computing system that serves as the gateway between hardware modules and the cloud.

## Overview

The In-Vehicle Compute system is an edge computing device (Raspberry Pi 4/5, Orange Pi, Jetson Nano, or similar) running containerized services that:
- Convert CAN bus messages to MQTT for local wireless communication (when internet unavailable) and cloud sync (when connected)
- Cache data locally for offline operation and autonomous vehicle control
- Distribute OTA firmware updates to hardware modules
- Provide local API for vehicle status queries
- Route commands from users/cloud to hardware modules via CAN

## Architecture

```
┌─────────────────────────────────────┐
│    In-Vehicle Compute (Edge)        │
│  (Raspberry Pi, Orange Pi, etc.)    │
├─────────────────────────────────────┤
│                                     │
│  ┌────────────────────────────────┐ │
│  │  Docker Container Services     │ │
│  ├────────────────────────────────┤ │
│  │ • CAN-to-MQTT Gateway          │ │
│  │ • MQTT Broker (Mosquitto)      │ │
│  │ • REST API Server              │ │
│  │ • Local Data Storage           │ │
│  │ • Configuration Manager        │ │
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

### 1. CAN-to-MQTT Gateway
Converts CAN messages to MQTT messages for internal routing and cloud sync.

### 2. Mosquitto MQTT Broker
Message broker for internal container communication and cloud connectivity.

### 3. REST API Server
Provides local API endpoints for status queries and commands.

### 4. Data Storage
SQLite database and file storage for local caching and offline operation.

### 5. Configuration Manager
Manages device configuration, CAN ID mappings, and module settings.

## Setup & Installation

### Prerequisites
- Raspberry Pi 4 or 5 (4GB+ RAM recommended)
- MicroSD card (32GB+)
- Power supply (adequate for Pi and CAN transceiver)
- Ethernet or WiFi connectivity
- CAN transceiver connected to GPIO

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
  can-gateway:      # CAN ↔ MQTT converter
  mosquitto:        # MQTT broker
  api:              # REST API server
  data-store:       # Local storage
  config:           # Configuration service
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

### Local SQLite Database
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
- Docker services: ~200MB total
- SQLite cache: ~50-100MB
- Free RAM: ~1.5-2GB

### Disk Usage
- System: ~2GB
- Database: ~1-5GB (with 24-48 hours data)
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

## Source Code

Vehicle Compute system source: `/Product/TrailCurrentPiCanToMqttAndDocker/`

---

See also:
- [01_Architecture/](../01_Architecture/) - System architecture
- [08_Deployment/](../08_Deployment/) - Deployment procedures
- [09_Troubleshooting/](../09_Troubleshooting/) - Common issues
