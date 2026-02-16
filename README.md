# TrailCurrentDocumentation
Documentation for the TrailCurrent Platform
# TrailCurrent Documentation

Welcome to the TrailCurrent Platform documentation. This is a comprehensive guide for understanding, developing, deploying, and maintaining the TrailCurrent system.

## ⚠️ CRITICAL LEGAL NOTICE

**TrailCurrent is a reference implementation project.** By using this system in any way, you:

1. **Acknowledge and Accept Full Liability** - You assume all responsibility for any damages, harm, or failures resulting from use of TrailCurrent
2. **Understand No Warranties** - TrailCurrent is provided "AS-IS" without any guarantees
3. **Accept No Support Guarantee** - The project, contributors, companies, and individuals are NOT responsible for any outcomes
4. **Agree to Test Thoroughly** - You must conduct extensive testing before any use
5. **Accept Safety Responsibility** - You must implement all necessary safety measures

**👉 [READ LIABILITY_AND_DISCLAIMER.md](LIABILITY_AND_DISCLAIMER.md) BEFORE PROCEEDING** - This is legally binding.

---

## What is TrailCurrent?

TrailCurrent is a **reference implementation** of an **Open Source Software Defined Vehicle (SDV) Platform** for comprehensive monitoring, control, autonomous decision-making, and intelligent management of mobile systems. It's designed to be broadly applicable to a wide variety of vehicle types including RVs, trailers, boats, aircraft, and other mobile platforms. Visit [trailcurrent.com](https://trailcurrent.com) to learn more about the platform and see it in action.

Unlike traditional IoT platforms, TrailCurrent is specifically architected for **autonomous vehicle intelligence** with:
- **Safety-first design** - Physical wired controls for all critical functions
- **Local autonomous intelligence** - AI/ML components running on edge compute making real-time decisions
- **Complete vehicle autonomy** - Full functionality without cloud or internet
- **Software-defined behavior** - Programmable, intelligent vehicle behavior via open-source software
- **Complete data sovereignty** - All data stays under user control, no vendor lock-in

**⚠️ This is NOT production-ready commercial software.** Users must conduct extensive testing, validation, and modification for their specific use cases. It consists of:

- **Multiple ESP32-based Hardware Modules** for sensing, monitoring, and control
- **In-Vehicle Compute System** for edge processing and gateway functionality (Raspberry Pi, Orange Pi, Jetson Nano, or similar)
- **Cloud Application** for remote monitoring, analytics, and management
- **Android Mobile App** for user interaction
- **Docker-based Infrastructure** for deployment and scaling

## Quick Navigation

- **⚖️ Legal Disclaimer** (REQUIRED READING!) → [LIABILITY_AND_DISCLAIMER.md](LIABILITY_AND_DISCLAIMER.md) - **You assume all liability**
- **🚗 What is Software Defined Vehicle?** → [WHAT_IS_SOFTWARE_DEFINED_VEHICLE.md](WHAT_IS_SOFTWARE_DEFINED_VEHICLE.md) - **Understand why TrailCurrent is different**
- **⭐ Core Principles** (READ FIRST!) → [CORE_PRINCIPLES.md](CORE_PRINCIPLES.md) - **Safety First • Cloud Optional • Data Private • 100% Open Source**
- **New to the System?** Start with [QUICK_START.md](QUICK_START.md)
- **System Architecture?** See [01_Architecture/PLATFORM_OVERVIEW.md](01_Architecture/PLATFORM_OVERVIEW.md) or the [interactive architecture overview](https://trailcurrent.com/architecture.html)
- **Hardware Modules?** Browse [02_Hardware_Modules/](02_Hardware_Modules/)
- **In-Vehicle Compute System?** Go to [03_Vehicle_Compute/](03_Vehicle_Compute/)
- **Cloud Application?** See [04_Cloud_Application/](04_Cloud_Application/)
- **Setting up Development?** Check [07_Development/](07_Development/)
- **Want to Contribute?** See [07_Development/CONTRIBUTING.md](07_Development/CONTRIBUTING.md)
- **Deployment Issues?** See [09_Troubleshooting/](09_Troubleshooting/)

## Documentation Structure

```
├── 01_Architecture/          - High-level system design and overview
├── 02_Hardware_Modules/      - ESP32 modules organized by function
├── 03_Vehicle_Compute/       - In-vehicle edge computing system
├── 04_Cloud_Application/     - Backend and frontend cloud services
├── 05_Mobile_Application/    - Android application
├── 06_Shared_Libraries/      - Common libraries and utilities
├── 07_Development/           - Development setup and guidelines
├── 08_Deployment/            - Deployment procedures and scripts
├── 09_Troubleshooting/       - Common issues and solutions
├── 10_Reference/             - API reference, CAN definitions, etc.
└── _images/                  - Diagrams and visual assets
```

## Key Concepts

### CAN Bus Network
Hardware modules communicate via CAN bus protocol for reliable, real-time data exchange.

### MQTT Gateway
The in-vehicle compute system converts CAN messages to MQTT, enabling both local wireless connectivity (when internet is unavailable) and cloud integration (when connected). This allows the vehicle to operate autonomously during travels through areas with no WAN connectivity, then sync data with the cloud when connection is restored.

### Docker Infrastructure
Services run in Docker containers for consistent deployment across environments.

### OTA Updates
Firmware updates are distributed over-the-air to hardware modules.

### Cloud is Optional
The in-vehicle compute system operates completely independently. Cloud connectivity is entirely optional - vehicles function with full autonomy using local MQTT messaging. When you choose to connect to a cloud, it's **your private cloud**, hosted wherever and however you prefer.

### Data Privacy First
All vehicle data stays on your hardware. TrailCurrent is designed to **never require or send data to third-party services**. There is no vendor lock-in, no cloud accounts with big tech companies, no tracking. Complete data sovereignty.

### 100% Open Source
Every component of TrailCurrent is open source: firmware, hardware designs (CAD/EDA), backend code, frontend code, tools, and utilities. This ensures transparency, prevents lock-in, and enables community contributions.

## Getting Started

1. **First Time?** Read [QUICK_START.md](QUICK_START.md)
2. **Understanding the System?** Review [01_Architecture/PLATFORM_OVERVIEW.md](01_Architecture/PLATFORM_OVERVIEW.md)
3. **Setting Up Development?** See [07_Development/DEVELOPMENT_SETUP.md](07_Development/DEVELOPMENT_SETUP.md)
4. **Ready to Deploy?** Check [08_Deployment/](08_Deployment/)

## Development Workflow

- **Adding a Module?** Start with [02_Hardware_Modules/README.md](02_Hardware_Modules/README.md)
- **Cloud Feature?** See [04_Cloud_Application/](04_Cloud_Application/)
- **Troubleshooting?** Consult [09_Troubleshooting/](09_Troubleshooting/)

## Links to Source Code

- **MCU Projects**: Parent folder `/Product/`
- **Shared Libraries**: `/Product/Libraries/`
- **Vehicle Compute**: `/Product/TrailCurrentPiCanToMqttAndDocker/`
- **Cloud App**: `/Product/TrailCurrentCloud/`
- **Android App**: `/Product/TrailCurrentAndroidApp/`
- **Deployment**: `/Product/TrailCurrentDeployment/`

## Contributing

Please see [07_Development/CONTRIBUTING.md](07_Development/CONTRIBUTING.md) for guidelines on contributing to the documentation.

## Support & Questions

For detailed answers to common questions, see [09_Troubleshooting/COMMON_ISSUES.md](09_Troubleshooting/COMMON_ISSUES.md).
