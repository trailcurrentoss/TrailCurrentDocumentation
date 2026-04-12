# TrailCurrent Deployment Architecture

Infrastructure and deployment topology for the TrailCurrent platform.

## Deployment Tiers

### Tier 1: Vehicle Installation

```
┌─────────────────────────────────────┐
│     Vehicle Environment             │
├─────────────────────────────────────┤
│                                     │
│  ┌────────────────────────┐         │
│  │  In-Vehicle Compute    │         │
│  │  (Raspberry Pi CM5)    │         │
│  │  - Raspberry Pi OS     │         │
│  │  - Docker & Compose    │         │
│  │  - 4GB+ RAM            │         │
│  │  - eMMC/SD 32GB+       │         │
│  └────────┬───────────────┘         │
│           │                         │
│           │ Ethernet + WiFi         │
│           │ Can be optionally       │
│           │ connected               │
│           │                         │
│  ┌────────▼──────────────┐          │
│  │ Waveshare RS485 CAN   │          │
│  │ HAT (B) - MCP2515/SPI │          │
│  └────────┬──────────────┘          │
│           │                         │
│           │ CAN Bus (Isolated)      │
│           │                         │
│    ┌──────┴────────┬──────────┐     │
│    │               │          │     │
│  ┌─▼──┐          ┌─▼──┐    ┌─▼──┐  │
│  │ M1 │          │ M2 │ .. │ Mn │  │
│  │ESP │          │ESP │    │ESP │  │
│  │32  │          │32  │    │32  │  │
│  └────┘          └────┘    └────┘  │
│                                     │
└─────────────────────────────────────┘
```

### Tier 2: Cloud Deployment (Farwatch — Optional)

```
┌──────────────────────────────────────────────────────┐
│            Cloud Host (any VPS)                       │
│            (Hetzner / DigitalOcean / AWS / on-prem)   │
├──────────────────────────────────────────────────────┤
│                                                      │
│  ┌─────────────────────────────────────┐            │
│  │  nginx (TLS termination)            │            │
│  │  Let's Encrypt via certbot           │            │
│  └────────────┬────────────────────────┘            │
│               │                                      │
│               ▼                                      │
│  ┌────────────────────────────────────┐              │
│  │  Frontend (vanilla JS PWA)         │              │
│  └────────────┬───────────────────────┘              │
│               │                                      │
│               ▼                                      │
│  ┌────────────────────────────────────┐              │
│  │  Backend (Node.js + Express)       │              │
│  │    • REST API                      │              │
│  │    • WebSocket fan-out             │              │
│  │    • API key auth                  │              │
│  │    • Deployment package storage    │              │
│  │    • Proximity automation engine   │              │
│  └────┬──────────────────────┬────────┘              │
│       │                      │                       │
│  ┌────▼────┐          ┌──────▼─────┐                 │
│  │ MongoDB │          │  Mosquitto │ ← MQTTS 8883    │
│  │ (state) │          │   broker   │                 │
│  └─────────┘          └──────┬─────┘                 │
│                              │                       │
│  ┌──────────────────┐        │                       │
│  │  tileserver-gl   │        │                       │
│  │  (vector tiles)  │        │                       │
│  └──────────────────┘        │                       │
└──────────────────────────────┼───────────────────────┘
                               │ MQTTS over cellular / LTE
                               ▼
                     Vehicle Headwaters bridge
```

> **Core Principle:** Farwatch is *optional*. A TrailCurrent vehicle is fully
> functional with no cloud connection. See [CORE_PRINCIPLES.md](../CORE_PRINCIPLES.md).

## Docker Container Architecture (In-Vehicle Compute)

```
┌──────────────────────────────────────────────────┐
│  Docker (In-Vehicle Compute Device)              │
├──────────────────────────────────────────────────┤
│                                                  │
│  ┌──────────────────────────────────────────┐   │
│  │  Docker Network (bridge): 172.20.0.0/16  │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ Backend Container                  │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: backend                   │ │   │
│  │  │ Port: 3000 (REST API)              │ │   │
│  │  │ Function: CAN↔MQTT + REST API      │ │   │
│  │  │ Language: Node.js                  │ │   │
│  │  │ Devices: /dev/can0 (MCP2515 SPI)   │ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ Frontend Container                 │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: frontend                  │ │   │
│  │  │ Port: 443 (HTTPS dashboard)        │ │   │
│  │  │ Function: Local web dashboard      │ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ Mosquitto MQTT Broker Container    │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: mosquitto                 │ │   │
│  │  │ Port: 1883 (internal), 8883 (TLS) │ │   │
│  │  │ Function: Message broker           │ │   │
│  │  │ Volumes: mosquitto-data,           │ │   │
│  │  │          mosquitto-log             │ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ MongoDB Container                  │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: mongodb                   │ │   │
│  │  │ Port: 27017 (internal)             │ │   │
│  │  │ Function: Local document store     │ │   │
│  │  │ Volumes: mongodb-data              │ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ Tile Server Container              │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: tileserver                │ │   │
│  │  │ Port: 8080 (internal)              │ │   │
│  │  │ Function: Offline vector map tiles │ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  └──────────────────────────────────────────┘   │
│                                                  │
└──────────────────────────────────────────────────┘

Host OS (Linux) Controls:
├─ Network (Ethernet/WiFi)
├─ CAN Interface (/dev/can0)
├─ GPIO for LED indicators
├─ System time and NTP
├─ Systemd service management
└─ SSH access
```

## Cloud Deployment Options

### Option A: Single VPS (Default)

```
┌─────────────────────────────┐
│   Single Cloud VPS           │
│  (2 vCPU / 4 GB / 40 GB)     │
├─────────────────────────────┤
│                             │
│  Docker Compose:            │
│  ├─ nginx + PWA (frontend)  │
│  ├─ Node.js backend         │
│  ├─ MongoDB                 │
│  ├─ Mosquitto (TLS)         │
│  ├─ tileserver-gl           │
│  └─ certbot (Let's Encrypt) │
│                             │
└─────────────────────────────┘
     ↑
     │ Internet (HTTPS + MQTTS 8883)
     ↓
One or more vehicles running Headwaters
```

This is the supported production topology for Farwatch. The same `docker-compose.yml`
runs at scale from a hobby deployment (one vehicle) up through small fleets
(tens of vehicles) without changes.

### Option B: Self-Hosted On-Premises

Replace the VPS with a local server behind your own firewall. The stack is
identical. Useful for organizations that cannot or do not want to expose
vehicle telemetry to a public cloud provider.

### Option C: No Cloud (Fully Local)

Skip Farwatch entirely. Headwaters on the vehicle already exposes the same
dashboard, REST API, WebSocket, and MQTT broker over the local network. Phones
and browsers on the vehicle WiFi talk directly to Headwaters. See [03_Vehicle_Compute/](../03_Vehicle_Compute/).

## Deployment Workflow

```
Developer Pushes Code
        ↓
CI/CD Pipeline (GitHub Actions / GitLab CI)
    ├─ Run tests
    ├─ Build Docker images
    ├─ Push to registry
    └─ Trigger deployment
        ↓
Staging Environment
    ├─ Deploy containers
    ├─ Run smoke tests
    └─ Manual QA (if needed)
        ↓
Production Environment
    ├─ Update containers
    ├─ Roll out with health checks
    ├─ Monitor metrics
    └─ Rollback if needed
        ↓
In-Vehicle Compute Updates (Via Deployment Script)
    ├─ Check for new firmware
    ├─ Download OTA packages
    ├─ Distribute via MQTT
    └─ Modules update and reboot
```

## Scaling Considerations

### Vertical Scaling (Larger VPS)

```
Upgrade server hardware:
├─ More RAM (MongoDB working set)
├─ Faster CPU (backend WebSocket fan-out)
├─ Faster storage (MongoDB I/O)
└─ Higher bandwidth (tileserver, deployment packages)
```

Because each vehicle owns its own data locally via Headwaters and Farwatch
only mirrors transient state, the cloud side scales mostly vertically with the
number of active dashboards and the size of the fleet of registered devices —
not with historical telemetry volume.

### Horizontal Scaling

Horizontal scaling (multiple backend replicas behind a load balancer, MongoDB
replica set) is supported but rarely necessary. If you are deploying Farwatch
to hundreds of vehicles, start with a larger VPS and only split services out
if you measure real contention.

### Geographic Distribution

```
Multiple Cloud Regions:
├─ Region 1 (Primary)
├─ Region 2 (Failover)
└─ Region 3 (Analytics/Reporting)

Vehicle Compute Locations:
├─ Vehicle 1 (Region 1)
├─ Vehicle 2 (Region 2)
├─ Vehicle 3 (Can connect to any region)
└─ ... (automatic failover)
```

## Monitoring & Logging

```
Each component logs to:
├─ Docker logs (stdout/stderr, preferred)
├─ MongoDB logs (inside the mongodb container)
├─ Mosquitto logs (inside the mosquitto container)
└─ Host system logs (journalctl)

Monitoring stack (optional):
├─ Prometheus (metrics collection)
├─ Grafana (visualization)
├─ AlertManager (alerts)
└─ Loki (log aggregation)
```

---

See also:
- [08_Deployment/](../08_Deployment/) - Deployment procedures
- [SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md) - Component architecture
- [03_Vehicle_Compute/](../03_Vehicle_Compute/) - Vehicle Pi setup
- [04_Cloud_Application/](../04_Cloud_Application/) - Cloud deployment
