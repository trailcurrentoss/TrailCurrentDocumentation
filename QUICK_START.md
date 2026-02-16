# TrailCurrent Quick Start Guide

Get up and running with TrailCurrent in 5 minutes.

## ⚠️ LEGAL NOTICE - READ FIRST

**Before proceeding, you MUST read and understand [LIABILITY_AND_DISCLAIMER.md](LIABILITY_AND_DISCLAIMER.md).**

By using TrailCurrent:
- You assume **all responsibility and liability** for any damages or harm
- You acknowledge **no warranties** are provided
- You accept that creators are **NOT responsible** for any outcomes
- You agree to **thoroughly test** before any use
- You accept **full safety responsibility** for your implementation

---

## Prerequisites

- Edge compute device (Raspberry Pi 4/5, Orange Pi, or similar for in-vehicle compute)
- One or more ESP32 development boards (for hardware modules)
- Docker and Docker Compose installed
- Node.js 24+ (for backend development)
- Basic understanding of networking and embedded systems

## 1. Understanding the System (2 min)

TrailCurrent is an **Open Source Software Defined Vehicle (SDV) Platform with Autonomous Intelligence** - not just another IoT project. It's specifically designed for intelligent vehicle control, autonomous decision-making, and comprehensive management. Visit [trailcurrent.com](https://trailcurrent.com) for a high-level overview of what TrailCurrent does and how it works.

**Three Essential Layers:**
1. **Vehicle Devices** (ESP32 modules) - Sensors, controllers, gateways communicating via hardwired CAN bus
2. **Vehicle Edge Compute** (Raspberry Pi/Orange Pi/etc.) - Autonomous intelligence system with local AI/ML for real-time decision-making, control, and autonomous operation
3. **Optional Cloud** (Self-hosted) - Remote monitoring, analytics, and management - completely optional, your private infrastructure

**Key SDV Design Principles:**
- ✅ **Vehicle Autonomy First** - Complete autonomous operation without internet or cloud (full intelligent control)
- ✅ **Local Autonomous Intelligence** - AI/ML components run locally on vehicle, making real-time autonomous decisions without cloud dependency
- ✅ **Safety-Critical Wired Control** - All critical functions have physical wired controls, never WiFi/cloud-only
- ✅ **Data Sovereignty** - Your data stays private, no third-party APIs or cloud lock-in
- ✅ **100% Open Source** - Complete transparency: firmware, CAD designs, schematics, all code

## 2. Your First Steps

Choose your path:

### I want to...

**...understand the overall architecture**
→ Read [01_Architecture/PLATFORM_OVERVIEW.md](01_Architecture/PLATFORM_OVERVIEW.md)

**...develop a hardware module**
→ See [02_Hardware_Modules/README.md](02_Hardware_Modules/README.md) and [07_Development/DEVELOPMENT_SETUP.md](07_Development/DEVELOPMENT_SETUP.md)

**...set up the in-vehicle compute system**
→ Follow [03_Vehicle_Compute/SETUP_GUIDE.md](03_Vehicle_Compute/SETUP_GUIDE.md)

**...deploy the cloud application**
→ Go to [04_Cloud_Application/SETUP_GUIDE.md](04_Cloud_Application/SETUP_GUIDE.md)

**...run the full system locally**
→ See [07_Development/DEVELOPMENT_SETUP.md](07_Development/DEVELOPMENT_SETUP.md)

**...deploy to production**
→ Check [08_Deployment/DEPLOYMENT_CHECKLIST.md](08_Deployment/DEPLOYMENT_CHECKLIST.md)

**...troubleshoot an issue**
→ Search [09_Troubleshooting/COMMON_ISSUES.md](09_Troubleshooting/COMMON_ISSUES.md)

## 3. Key Files & Locations

| Component | Location | Key Files |
|-----------|----------|-----------|
| Hardware Modules | `/Product/TrailCurrent*/` | `main/main.c`, `CMakeLists.txt` |
| Shared Libraries | `/Product/Libraries/` | Various library folders |
| Vehicle Compute | `/Product/TrailCurrentPiCanToMqttAndDocker/` | `docker-compose.yml`, containers/ |
| Cloud Backend | `/Product/TrailCurrentCloud/backend/` | `app.js`, routes/ |
| Cloud Frontend | `/Product/TrailCurrentCloud/frontend/` | HTML, CSS, JS files |
| Deployment Config | `/Product/TrailCurrentDeployment/` | docker/, config/, scripts/ |

## 4. Common Commands

```bash
# Build a hardware module
cd /Product/TrailCurrent[ModuleName]
idf.py build

# Run cloud application locally
cd /Product/TrailCurrentCloud
docker-compose -f docker-compose.dev.yml up

# Deploy to vehicle
cd /Product/TrailCurrentDeployment
./deploy.sh

# Check system status
docker ps  # See running containers
```

## 5. Next Steps

1. **Read the Architecture** - Understand how components fit together
2. **Choose Your Component** - Pick what you want to work on
3. **Follow Component Docs** - Each component has detailed setup guides
4. **Join Development** - See [07_Development/CONTRIBUTING.md](07_Development/CONTRIBUTING.md)

## Useful Documentation Links

- **System Overview**: [01_Architecture/SYSTEM_ARCHITECTURE.md](01_Architecture/SYSTEM_ARCHITECTURE.md)
- **All Hardware Modules**: [02_Hardware_Modules/MODULE_CATEGORIES.md](02_Hardware_Modules/MODULE_CATEGORIES.md)
- **Network Setup**: [01_Architecture/NETWORK_TOPOLOGY.md](01_Architecture/NETWORK_TOPOLOGY.md)
- **Troubleshooting**: [09_Troubleshooting/README.md](09_Troubleshooting/README.md)

---

**Lost?** Check the [main README.md](README.md) for more navigation options, or browse the documentation index in [10_Reference/](10_Reference/).
