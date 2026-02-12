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
│  │  (RPi, Orange Pi, etc.)│         │
│  │  - Linux OS            │         │
│  │  - Docker & Compose    │         │
│  │  - 4GB+ RAM            │         │
│  │  - Storage 32GB+       │         │
│  └────────┬───────────────┘         │
│           │                         │
│           │ Ethernet + WiFi         │
│           │ Can be optionally       │
│           │ connected               │
│           │                         │
│  ┌────────▼──────────────┐          │
│  │ CAN Transceiver       │          │
│  │ (SPI/GPIO interface)  │          │
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

### Tier 2: Cloud Deployment

```
┌──────────────────────────────────────────────────────┐
│            Cloud Infrastructure                       │
│         (AWS / Azure / On-Premise)                    │
├──────────────────────────────────────────────────────┤
│                                                      │
│  ┌─────────────────────────────────────┐            │
│  │  Load Balancer / API Gateway        │            │
│  │  (SSL/TLS Termination)              │            │
│  └────────────┬────────────────────────┘            │
│               │                                      │
│  ┌────────────┼─────────────────────┐               │
│  │            │                     │               │
│  ▼            ▼                     ▼               │
│ ┌──────┐  ┌──────┐  ┌──────────┐                   │
│ │ API  │  │ API  │  │ API      │  (Replicas)      │
│ │ #1   │  │ #2   │  │ #n       │                   │
│ └──┬───┘  └──┬───┘  └──┬───────┘                   │
│    │         │         │                           │
│    └────┬────┴────┬────┘                           │
│         │         │                                │
│    ┌────▼─────────▼──────┐                        │
│    │  Shared Services    │                        │
│    ├─────────────────────┤                        │
│    │ PostgreSQL (DB)     │                        │
│    │ Redis (Cache)       │                        │
│    │ Mosquitto (MQTT)    │                        │
│    │ File Storage        │                        │
│    └─────────────────────┘                        │
│                                                      │
│  ┌──────────────────────────────────┐              │
│  │  Web Server (Static)              │              │
│  │  Serves HTML/CSS/JS               │              │
│  └──────────────────────────────────┘              │
│                                                      │
└──────────────────────────────────────────────────────┘
```

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
│  │  │ CAN-to-MQTT Gateway Container      │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: can-gateway               │ │   │
│  │  │ Port: 8082 (internal metrics)      │ │   │
│  │  │ Function: Convert CAN → MQTT       │ │   │
│  │  │ Volumes: /dev/ttyUSB0 (CAN device)│ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ Mosquitto MQTT Broker Container    │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: mosquitto                 │ │   │
│  │  │ Port: 1883 (internal), 8883 (TLS) │ │   │
│  │  │ Function: Message broker           │ │   │
│  │  │ Volumes: /etc/mosquitto/config    │ │   │
│  │  │          /var/lib/mosquitto/data  │ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ REST API / Backend Container       │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: api-server                │ │   │
│  │  │ Port: 3000 (external: 8080)        │ │   │
│  │  │ Function: API endpoints            │ │   │
│  │  │ Language: Node.js                  │ │   │
│  │  │ Volumes: /app/data (persistence)  │ │   │
│  │  │          /app/config               │ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ SQLite / Local Storage Container   │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: data-store                │ │   │
│  │  │ Port: None (internal)              │ │   │
│  │  │ Function: Local data cache         │ │   │
│  │  │ Volumes: /data/sqlite.db           │ │   │
│  │  │          /data/cache               │ │   │
│  │  └────────────────────────────────────┘ │   │
│  │                                          │   │
│  │  ┌────────────────────────────────────┐ │   │
│  │  │ Configuration Container            │ │   │
│  │  ├────────────────────────────────────┤ │   │
│  │  │ Service: config-manager            │ │   │
│  │  │ Port: None (internal)              │ │   │
│  │  │ Function: Manage app configuration │ │   │
│  │  │ Volumes: /etc/trailcurrent/        │ │   │
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

### Option A: Single Server (Small Deployment)

```
┌─────────────────────────────┐
│   Single Cloud Server        │
│  (t2.medium AWS / similar)   │
├─────────────────────────────┤
│                             │
│  All services in containers:│
│  ├─ API Server              │
│  ├─ Web UI                  │
│  ├─ MQTT Broker             │
│  ├─ PostgreSQL Database     │
│  ├─ Redis Cache             │
│  └─ File Storage            │
│                             │
│  Docker Compose manages all │
│                             │
└─────────────────────────────┘
     ↑
     │ Internet
     ↓
Multiple Vehicles (In-Vehicle Compute Devices)
```

### Option B: Multi-Container (Medium Deployment)

```
┌──────────────────────────────────────┐
│   Kubernetes or Docker Swarm Cluster │
├──────────────────────────────────────┤
│                                      │
│  ┌─────────────┐  ┌─────────────┐   │
│  │ Node 1      │  │ Node 2      │   │
│  ├─────────────┤  ├─────────────┤   │
│  │ API Pod #1  │  │ API Pod #2  │   │
│  │ API Pod #3  │  │ API Pod #4  │   │
│  └─────────────┘  └─────────────┘   │
│                                      │
│  ┌────────────────────────────────┐  │
│  │ Persistent Storage             │  │
│  ├────────────────────────────────┤  │
│  │ PostgreSQL (replicated)        │  │
│  │ Redis (clustered)              │  │
│  │ MQTT Broker (clustered)        │  │
│  │ File Storage (S3 / NAS)        │  │
│  └────────────────────────────────┘  │
│                                      │
└──────────────────────────────────────┘
```

### Option C: Managed Services (Production)

```
┌──────────────────────────────────────┐
│  AWS / Azure / GCP Services          │
├──────────────────────────────────────┤
│                                      │
│  Container Management:               │
│  ├─ ECS / EKS / AKS                 │
│  └─ Managed load balancing          │
│                                      │
│  Data Services:                      │
│  ├─ RDS (PostgreSQL managed)        │
│  ├─ ElastiCache (Redis managed)     │
│  └─ S3 / Blob Storage               │
│                                      │
│  Networking:                         │
│  ├─ CloudFront / CDN                │
│  ├─ VPC / Virtual Network           │
│  └─ Certificate Management (ACM)    │
│                                      │
└──────────────────────────────────────┘
```

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

### Horizontal Scaling (Add more instances)

```
Load Balancer
├─ API Server #1
├─ API Server #2
├─ API Server #3
└─ ... (add more as needed)

Database:
├─ PostgreSQL Primary
└─ PostgreSQL Read Replicas
    └─ Handles read-heavy loads
```

### Vertical Scaling (Larger instances)

```
Upgrade server hardware:
├─ More RAM (for Redis cache)
├─ Faster CPU (for API processing)
├─ Faster storage (for database I/O)
└─ Higher bandwidth
```

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
├─ Docker logs (stdout/stderr)
├─ Application logs (files in /app/logs/)
├─ Database logs (/var/log/postgresql/)
└─ System logs (journalctl)

Monitoring stack (optional):
├─ Prometheus (metrics collection)
├─ Grafana (visualization)
├─ AlertManager (alerts)
└─ ELK Stack (log aggregation)
```

---

See also:
- [08_Deployment/](../08_Deployment/) - Deployment procedures
- [SYSTEM_ARCHITECTURE.md](SYSTEM_ARCHITECTURE.md) - Component architecture
- [03_Vehicle_Compute/](../03_Vehicle_Compute/) - Vehicle Pi setup
- [04_Cloud_Application/](../04_Cloud_Application/) - Cloud deployment
