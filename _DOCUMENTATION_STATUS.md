# TrailCurrent Documentation Status

## Overview

This document tracks the status of TrailCurrent documentation creation and completion.

**Last Updated**: 2026-04-11
**Total Markdown Files**: 46
**Status**: Core structure complete. CAN Bus Reference refreshed from DBC (now includes Reservoir, expanded Torrent/Solstice IDs, BorealisCalibration). Hardware module docs reflect the full ESP-IDF migration (all modules except Spotter) and the new Reservoir water-tank module. Cloud Application README rewritten to match Farwatch (MongoDB, API key auth, PWA). Mobile Application README now covers both Outbound (Android native) and the React Native app. Vehicle Compute README updated to CM5 + carrier board hardware. BUILD_SYSTEM.md ESP-IDF section filled in.

### Recent sweep (2026-04-11 — second pass)

Second pass across the remaining areas:

- `01_Architecture/DEPLOYMENT_ARCHITECTURE.md` — cloud tier rewritten around Farwatch (single-VPS default; MongoDB instead of PostgreSQL+Redis; Let's Encrypt; proximity automation engine). Scaling guidance is now "vertical by default, horizontal only if measured"
- `01_Architecture/PLATFORM_OVERVIEW.md` — cloud components updated to Farwatch (MongoDB + tileserver + PWA); hardware module list refreshed to include Reservoir and Switchback; Key Technologies table refreshed with ESP-IDF v5.1+ and MongoDB
- `01_Architecture/SYSTEM_ARCHITECTURE.md` — cloud tier diagram updated (Farwatch: Backend, MongoDB, Mosquitto TLS, tileserver, package storage); scalability section rewritten around MongoDB replica sets and backend replicas
- `01_Architecture/DATA_FLOW.md` — added Scenario 3 (Reservoir water tanks) and Scenario 4 (Proximity automation, phone ↔ vehicle via Farwatch); Scenario 3 "Offline Operation" renumbered to Scenario 5
- `06_Shared_Libraries/README.md` — reflects ESP-IDF migration; C6-specific libraries documented as retired (modules moved to Waveshare S3 boards); remaining WROOM32 libraries scoped to Torrent/Tapper
- `08_Deployment/README.md` — "Infrastructure Scaling" rewritten around vertical scaling (default) + MongoDB replica set + backend replicas
- `10_Reference/GLOSSARY.md` — PostgreSQL/Redis/JWT entries replaced with MongoDB and API Key; CAN Transceiver entry corrected (SN65HVD230 on modules, MCP2515 HAT on Headwaters)
- `10_Reference/README.md` — network ports table updated (MongoDB 27017, tileserver 8080, MQTT TLS 8883 annotation for vehicle↔cloud)
- `10_Reference/HARDWARE_SPECIFICATIONS.md` — CAN Transceiver section filled in: SN65HVD230 on modules, MCP2515 on the Waveshare RS485 CAN HAT (B) for Headwaters
- `10_Reference/EXTERNAL_LINKS.md` — Database Tools section populated with MongoDB references (Manual, mongosh, Compass, backup docs)

### Recent sweep (2026-04-11 — first pass)

Documentation drift was audited against the current state of every project under
`/Product/` and the following files were brought up to date:

- `02_Hardware_Modules/README.md` — framework claims corrected (all ESP-IDF except Spotter); Reservoir added; CAN ID map expanded; hardware variant table rewritten; source paths fixed (Bearing was referencing the old `TrailCurrentGnssModule/` path)
- `02_Hardware_Modules/MODULE_CATEGORIES.md` — all CAN ID ranges corrected to the real DBC values; Reservoir + Switchback added as full entries; Peregrine updated to reflect the Radxa Dragon Q6A + offline pipeline
- `02_Hardware_Modules/Firmware/PlatformIO_Setup.md` — marked legacy / Spotter-only
- `10_Reference/CAN_BUS_REFERENCE.md` — message table updated (added BorealisCalibration, ShuntExt, SolarLoadControl, TorrentSequence, WaterTankLevels, per-address Torrent and Switchback rows); Bus Nodes table now includes Reservoir and fixes Headwaters, Aftline, Switchback, Borealis hardware
- `03_Vehicle_Compute/README.md` — overview rewritten to explicitly reference CM5 + Waveshare RS485 CAN HAT (B) and to point at Baseflow as the lite variant
- `04_Cloud_Application/README.md` — fully rewritten to match Farwatch reality (MongoDB not PostgreSQL, API key auth not JWT, PWA + WebSocket architecture, proximity automation, deployment packages, data minimization stance)
- `05_Mobile_Application/README.md` — fully rewritten to cover both TrailCurrent Outbound (Android native, Kotlin + Jetpack Compose + MapLibre) and the React Native / Expo app
- `07_Development/BUILD_SYSTEM.md` — ESP-IDF section and backend build section filled in (replacing NEEDS TO BE COMPLETED stubs)

---

## Documentation Categories

### ✅ Root Documentation (5/5 Complete)

Core project documentation providing overview and guidance:

- [x] **README.md** - Main entry point with platform overview
- [x] **CORE_PRINCIPLES.md** - Foundational principles (Cloud Optional, Data Private, 100% Open Source, Safety First)
- [x] **QUICK_START.md** - 5-minute getting started guide
- [x] **WHAT_IS_SOFTWARE_DEFINED_VEHICLE.md** - Why TrailCurrent differs from generic IoT
- [x] **LIABILITY_AND_DISCLAIMER.md** - Legal notice and disclaimer

### ✅ Architecture Documentation (5/5 Complete)

System design and architecture:

- [x] **01_Architecture/PLATFORM_OVERVIEW.md** - System layers and architecture
- [x] **01_Architecture/SYSTEM_ARCHITECTURE.md** - Detailed system design
- [x] **01_Architecture/NETWORK_TOPOLOGY.md** - Network layout and connectivity
- [x] **01_Architecture/DATA_FLOW.md** - Information flow through system
- [x] **01_Architecture/DEPLOYMENT_ARCHITECTURE.md** - Infrastructure design

### ✅ Hardware Module Documentation (4/4 Complete)

Hardware module documentation:

- [x] **02_Hardware_Modules/README.md** - Hardware overview and module list (17 modules, now includes Reservoir)
- [x] **02_Hardware_Modules/MODULE_CATEGORIES.md** - Module classification and organization
- [x] **02_Hardware_Modules/Firmware/ESP_IDF_Setup.md** - ESP-IDF firmware setup (primary setup for all modules)
- [x] **02_Hardware_Modules/Firmware/PlatformIO_Setup.md** - PlatformIO setup (legacy; Spotter-only)

**📋 Placeholder/Stub Files Needed** (16 module-specific files):
- Bearing (GNSS) documentation
- Borealis (environment) documentation
- Torrent (power delivery) documentation
- Therma (closed-loop thermostat) documentation
- Solstice (solar controller) documentation
- Ampline (shunt interface) documentation
- Plateau (vehicle level) documentation
- Picket (cabinet & door sensors) documentation
- Switchback (relay module) documentation
- Aftline (trailer wiring) documentation
- Reservoir (water tank levels) documentation
- Tapper (8-button panel) documentation
- Fireside (wireless display) documentation
- Milepost (hardwired display) documentation
- Spotter (trailer monitor display) documentation
- Peregrine (voice assistant) documentation

### ✅ In-Vehicle Compute Documentation (3/3 Complete)

Edge computing system documentation:

- [x] **03_Vehicle_Compute/README.md** - Edge compute overview (emphasizes local operation & optional cloud)
- [x] **03_Vehicle_Compute/SETUP_GUIDE.md** - Step-by-step setup guide
- [x] **03_Vehicle_Compute/Deployment/PI_DEPLOYMENT.md** - Deployment procedures

### ✅ Cloud Application Documentation (2/2 Complete)

Cloud services documentation:

- [x] **04_Cloud_Application/README.md** - Cloud application overview
- [x] **04_Cloud_Application/Deployment/CLOUD_DEPLOYMENT.md** - Cloud deployment guide

### ✅ Mobile Application Documentation (1/1 Complete)

Android application documentation:

- [x] **05_Mobile_Application/README.md** - Mobile app overview

### ✅ Shared Libraries Documentation (1/1 Complete)

Common libraries and utilities:

- [x] **06_Shared_Libraries/README.md** - Shared libraries overview

### ✅ Development Documentation (5/5 Complete)

Developer guidelines and procedures:

- [x] **07_Development/README.md** - Development overview
- [x] **07_Development/CONTRIBUTING.md** - Contribution guidelines (with Core Principles enforcement)
- [x] **07_Development/DEVELOPMENT_SETUP.md** - Development environment setup
- [x] **07_Development/BUILD_SYSTEM.md** - Build procedures and optimization
- [x] **07_Development/TESTING_GUIDE.md** - Testing frameworks and procedures
- [x] **07_Development/DEBUGGING.md** - Debugging techniques and tools

### ✅ Deployment Documentation (5/5 Complete)

Deployment and operation procedures:

- [x] **08_Deployment/README.md** - Deployment overview
- [x] **08_Deployment/DEPLOYMENT_CHECKLIST.md** - Pre-deployment verification
- [x] **08_Deployment/DEPLOYMENT_SCRIPTS.md** - Automation scripts reference
- [x] **08_Deployment/FIRMWARE_UPDATES.md** - OTA update procedures
- [x] **08_Deployment/DOCKER_SETUP.md** - Docker configuration guide

### ✅ Troubleshooting Documentation (5/5 Complete)

Diagnostic and problem-solving guides:

- [x] **09_Troubleshooting/README.md** - Troubleshooting overview
- [x] **09_Troubleshooting/COMMON_ISSUES.md** - Common problems and solutions
- [x] **09_Troubleshooting/HARDWARE_ISSUES.md** - Hardware troubleshooting guide
- [x] **09_Troubleshooting/NETWORK_ISSUES.md** - Network troubleshooting guide
- [x] **09_Troubleshooting/FIRMWARE_ISSUES.md** - Firmware troubleshooting guide
- [x] **09_Troubleshooting/LOGGING_AND_DIAGNOSTICS.md** - Diagnostic procedures

### ✅ Reference Documentation (7/7 Complete)

Technical reference materials:

- [x] **10_Reference/README.md** - Reference overview
- [x] **10_Reference/GLOSSARY.md** - Technical terminology and acronyms
- [x] **10_Reference/CAN_BUS_REFERENCE.md** - CAN message formats and IDs (fully populated from DBC)
- [x] **10_Reference/MQTT_TOPICS.md** - MQTT topic hierarchy and messages
- [x] **10_Reference/GPIO_PIN_MAPPING.md** - Pin assignments and wiring
- [x] **10_Reference/HARDWARE_SPECIFICATIONS.md** - Electrical and physical specs
- [x] **10_Reference/EXTERNAL_LINKS.md** - External resources and documentation
- [x] **TrailCurrent.dbc** - Machine-readable CAN database file (authoritative source for all CAN message definitions)

---

## Documentation Structure Summary

```
TrailCurrent Documentation (43 total files)
├── Root Documents (5)
│   ├── README.md
│   ├── CORE_PRINCIPLES.md ⭐ CRITICAL
│   ├── QUICK_START.md
│   ├── WHAT_IS_SOFTWARE_DEFINED_VEHICLE.md
│   └── LIABILITY_AND_DISCLAIMER.md
├── 01_Architecture (5)
│   ├── PLATFORM_OVERVIEW.md
│   ├── SYSTEM_ARCHITECTURE.md
│   ├── NETWORK_TOPOLOGY.md
│   ├── DATA_FLOW.md
│   └── DEPLOYMENT_ARCHITECTURE.md
├── 02_Hardware_Modules (4 + 15 placeholders)
│   ├── README.md
│   ├── MODULE_CATEGORIES.md
│   ├── Firmware/ESP_IDF_Setup.md
│   └── Firmware/PlatformIO_Setup.md
├── 03_Vehicle_Compute (3)
│   ├── README.md
│   ├── SETUP_GUIDE.md
│   └── Deployment/PI_DEPLOYMENT.md
├── 04_Cloud_Application (2)
│   ├── README.md
│   └── Deployment/CLOUD_DEPLOYMENT.md
├── 05_Mobile_Application (1)
│   └── README.md
├── 06_Shared_Libraries (1)
│   └── README.md
├── 07_Development (6) ⭐ CONTRIBUTING.md is CRITICAL
│   ├── README.md
│   ├── CONTRIBUTING.md ⭐ ENFORCES CORE PRINCIPLES
│   ├── DEVELOPMENT_SETUP.md
│   ├── BUILD_SYSTEM.md
│   ├── TESTING_GUIDE.md
│   └── DEBUGGING.md
├── 08_Deployment (5)
│   ├── README.md
│   ├── DEPLOYMENT_CHECKLIST.md
│   ├── DEPLOYMENT_SCRIPTS.md
│   ├── FIRMWARE_UPDATES.md
│   └── DOCKER_SETUP.md
├── 09_Troubleshooting (6)
│   ├── README.md
│   ├── COMMON_ISSUES.md
│   ├── HARDWARE_ISSUES.md
│   ├── NETWORK_ISSUES.md
│   ├── FIRMWARE_ISSUES.md
│   └── LOGGING_AND_DIAGNOSTICS.md
├── 10_Reference (7)
│   ├── README.md
│   ├── GLOSSARY.md
│   ├── CAN_BUS_REFERENCE.md ⭐ FULLY POPULATED
│   ├── MQTT_TOPICS.md
│   ├── GPIO_PIN_MAPPING.md
│   ├── HARDWARE_SPECIFICATIONS.md
│   └── EXTERNAL_LINKS.md
└── TrailCurrent.dbc ⭐ AUTHORITATIVE CAN DATABASE
```

---

## Key Principles Embedded

### ⭐ Core Principles (All 4 Enforced)

1. **☁️ Cloud Optional** - Documented in every architecture and deployment guide
2. **🔐 Data Privacy First** - Emphasized in CONTRIBUTING.md and deployment guides
3. **📖 100% Open Source** - Required in CONTRIBUTING.md and design guidelines
4. **⚠️ Safety First** - Physical wired control required for all critical devices

### 🎯 Critical Documents

- **CORE_PRINCIPLES.md** - Non-negotiable principles guard against architectural violations
- **CONTRIBUTING.md** - Enforces principles in code reviews with explicit checklists
- **PLATFORM_OVERVIEW.md** - Makes Cloud layer optional and private
- All deployment docs emphasize local operation first, cloud sync secondary

---

## Documentation Completion Status

### Fully Completed (with content)
✅ **23 Files** - These files contain full structure, content outlines, and detailed organization:
- Root documents (3)
- Architecture (5)
- Development (6)
- Troubleshooting (6)
- Deployment (5)
- Reference (7)
- Component overviews (3)

### Placeholder/Stub Structure (ready for content)
📋 **20 Files** - These files have complete structure and "NEEDS TO BE COMPLETED" sections indicating what should go in each:
- Hardware module specifics (14)
- DEVELOPMENT_SETUP.md subsections
- Various deployment/troubleshooting subsections
- Reference file subsections

---

## What Each File Type Contains

### Reference Files
- Purpose statement
- Overview/Introduction (placeholder for completion)
- Section headings for expected content
- "NEEDS TO BE COMPLETED" blocks indicating what information belongs where
- Related documentation links
- Cross-references to other docs

### Development Guides
- Full structure with detailed headings
- Prerequisites and setup steps
- Step-by-step procedures
- Troubleshooting sections
- Related documentation links
- Best practices and guidelines

### Troubleshooting Guides
- Symptoms identification
- Diagnosis procedures
- Solution steps
- Tool references
- Related documentation
- Examples of correct vs incorrect

### Deployment Checklists
- Complete verification lists
- Go/no-go decision points
- Pre, during, and post-deployment phases
- Safety procedures
- Rollback procedures

---

## Next Steps for Documentation Completion

### Priority 1 (Referenced 5+ times)
Complete these to resolve most broken links:
- [x] CAN_BUS_REFERENCE.md - message formats ✅ Fully populated from DBC (2026-03-14)
- [ ] MQTT_TOPICS.md - topic definitions
- [ ] GPIO_PIN_MAPPING.md - complete pin assignments per module
- [ ] HARDWARE_SPECIFICATIONS.md - electrical specs
- [ ] COMMON_ISSUES.md - solutions for each issue type

### Priority 2 (Referenced 3-4 times)
Expand these key development guides:
- [ ] DEVELOPMENT_SETUP.md - system-specific setup
- [ ] BUILD_SYSTEM.md - build procedures for each platform
- [ ] TESTING_GUIDE.md - framework and execution
- [ ] DEBUGGING.md - tool-specific procedures

### Priority 3 (Referenced 1-2 times)
Complete hardware module-specific documentation:
- [ ] Bearing.md (GNSS module)
- [ ] Borealis.md (environment module)
- [ ] Torrent.md (power delivery module)
- [ ] [11 other module-specific files - see 02_Hardware_Modules/README.md]

### Priority 4 (Supporting Documentation)
Additional content that enhances but isn't critical:
- [ ] CONFIG_FILES.md - Configuration reference
- [ ] API_DOCUMENTATION.md - REST API reference
- [ ] MQTT_PROTOCOL.md - Protocol specifics

---

## Documentation Quality Metrics

| Metric | Status | Notes |
|--------|--------|-------|
| All Core Principles enforced | ✅ Complete | CORE_PRINCIPLES.md + CONTRIBUTING.md |
| Cloud optionality clear | ✅ Complete | Every architecture doc emphasizes local first |
| Safety requirements documented | ✅ Complete | Wired CAN control mandatory in CONTRIBUTING |
| Link structure (broken links identified) | ✅ Complete | All links mapped, placeholders created |
| Architecture documentation | ✅ Complete | 5 comprehensive architecture files |
| Development guidelines | ✅ Complete | 6 files covering setup, build, test, debug |
| Deployment procedures | ✅ Complete | 5 files covering all deployment phases |
| Troubleshooting guides | ✅ Complete | 6 organized troubleshooting categories |
| Reference materials | ✅ Complete | 7 reference files with frameworks |

---

## Statistics

- **Total Files Created**: 46 (43 markdown + 1 DBC + 2 new)
- **Files with Full Content**: 26
- **Files with Complete Structure/Placeholders**: 20
- **Total Documentation Pages** (estimated): 80-100 pages
- **Cross-references**: 100+
- **Principle Enforcement Points**: 15+ (in CONTRIBUTING.md and CORE_PRINCIPLES.md)
- **Deployment Checklists**: 1 comprehensive (DEPLOYMENT_CHECKLIST.md)
- **Code Examples Documented**: Multiple (in setup and deployment guides)

---

## How to Use This Documentation

1. **First Time Users**: Start with README.md → QUICK_START.md
2. **Architects**: Read CORE_PRINCIPLES.md → 01_Architecture/PLATFORM_OVERVIEW.md
3. **Developers**: CONTRIBUTING.md → 07_Development/DEVELOPMENT_SETUP.md
4. **Operations**: 08_Deployment/README.md → DEPLOYMENT_CHECKLIST.md
5. **Troubleshooting**: 09_Troubleshooting/COMMON_ISSUES.md
6. **Reference Lookup**: 10_Reference/GLOSSARY.md + specific reference docs

---

## Notes

- All created files are production-ready scaffolding with clear content organization
- "NEEDS TO BE COMPLETED" sections guide future writers on what belongs in each section
- All links between documents are properly configured for future content
- Core principles are front-and-center to prevent architectural violations
- Placeholder files follow the same structure as completed files for consistency

---

**Status**: Documentation framework complete and ready for incremental content population.
