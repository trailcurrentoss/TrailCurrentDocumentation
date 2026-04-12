# Build System Guide

Guide to building all TrailCurrent components.

## Overview

TrailCurrent has three distinct build domains:

| Domain | Framework | Output | Where it runs |
|--------|-----------|--------|---------------|
| **Hardware modules** | ESP-IDF (native CMake) | `.bin` + merged flash image | Flashed to ESP32 family MCUs |
| **Vehicle compute (Headwaters)** and **cloud (Farwatch)** | Docker Compose (multi-arch) | Container images | Raspberry Pi CM5 (vehicle) / VPS (cloud) |
| **Mobile apps** | Gradle (Outbound) / Expo EAS (React Native app) | APK/AAB or iOS/Android bundles | User's phone |

Spotter is the only hardware module that uses PlatformIO, and even it wraps
ESP-IDF underneath. All other ESP32 modules build directly with `idf.py`.

## Hardware Module Builds

### ESP-IDF Build System

All hardware modules are built with **ESP-IDF v5.1+** (most track v5.5).

```bash
# One-time: source the ESP-IDF environment
. $HOME/esp/esp-idf/export.sh

# Clean build
idf.py fullclean

# Compile
idf.py build

# Flash + monitor
idf.py -p /dev/ttyUSB0 flash monitor
```

Each module has a `sdkconfig.defaults` that the build expands into `sdkconfig`
on first build. Changes to `sdkconfig` are local; changes that should be
tracked go in `sdkconfig.defaults`. Modules also carry
`dependencies.lock` — a lockfile for the ESP-IDF component manager. Do not
edit it by hand; regenerate with `idf.py reconfigure`.

### Building Individual Modules

Each hardware project is a self-contained ESP-IDF repo:

```
TrailCurrent<Module>/
├── CMakeLists.txt           # top-level project
├── main/                    # application component
│   ├── CMakeLists.txt
│   └── *.c / *.h
├── managed_components/      # auto-populated by component manager
├── sdkconfig.defaults       # tracked Kconfig defaults
├── partitions.csv           # custom partition layout (dual OTA + factory)
└── build/                   # build output (gitignored)
```

### Multi-Instance Builds

Modules that need multiple instances on the same CAN bus ship a
`build-all.sh` script that produces one binary per address / target. See
[MULTI_INSTANCE_MODULES.md](./MULTI_INSTANCE_MODULES.md) for the full pattern.

| Module | `build-all.sh` outputs |
|--------|-----------------------|
| Torrent | `torrent_addr0.bin` .. `torrent_addr2.bin` |
| Switchback | `switchback_addr0.bin` .. `switchback_addr2.bin` |
| Picket | `picket_addr0.bin` .. `picket_addr7.bin` |
| Tapper | `tapper_torrent_addr0.bin` .. `tapper_switchback_addr2.bin` |
| Therma | `therma_controller.bin`, `therma_heater_relay.bin`, `therma_cooler_relay.bin` |

Several modules (Bearing, Torrent, Switchback, Tapper, Therma, Reservoir) also
publish a `*_merged.bin` — bootloader + partition table + OTA data + app
stitched into a single image suitable for the web flasher.

### Binary Output

After `idf.py build` the binaries are written to `build/`:

| File | Purpose |
|------|---------|
| `<project>.bin` | Application image (what OTA uploads) |
| `<project>.elf` | Unstripped ELF with symbols (used by `idf.py monitor` for stack traces) |
| `<project>.map` | Linker map for size analysis |
| `bootloader/bootloader.bin` | Second-stage bootloader |
| `partition_table/partition-table.bin` | Partition layout |
| `ota_data_initial.bin` | Blank OTA data partition |

Version reporting on the CAN bus uses the `FirmwareVersionReport` message
(`0x04`) emitted at boot. Version strings come from `git describe` at build
time.

## Backend Build (Headwaters / Farwatch / Baseflow)

All three backends are Node.js + Express services packaged as Docker
containers via `docker compose build`. They share most of the same Dockerfile
layout:

```bash
cd /Product/TrailCurrentHeadwaters   # or TrailCurrentFarwatch / TrailCurrentBaseflow
docker compose build                 # build all services for the host arch
docker compose build --no-cache backend
docker buildx bake --set *.platform=linux/arm64    # cross-build for CM5
```

The vehicle compute (Headwaters) uses **multi-arch** buildx bakes because
images are cross-built for `linux/arm64` (CM5) on an `x86_64` workstation.

### Database (MongoDB)

Both Headwaters and Farwatch use MongoDB. There is no traditional schema
migration step — the backend validates documents at write time. Seed data
(default settings, example automation rules) is loaded on first boot when
collections are empty.

## Frontend Build

### Build Optimization

**NEEDS TO BE COMPLETED** - Document:
- Minification
- CSS optimization
- JavaScript bundling (if used)
- Image optimization
- Asset compression

### Development vs Production Build

**NEEDS TO BE COMPLETED** - Compare:
- Development build features
- Production optimizations
- Build size differences
- Performance differences
- Feature availability

## Docker Builds

### Building Docker Images

**NEEDS TO BE COMPLETED** - Document:
- Dockerfile structure
- Multi-stage builds
- Base images
- Layer caching
- Build arguments
- Build tags

### Building Images for Different Platforms

**NEEDS TO BE COMPLETED** - Document:
- ARM vs x86
- Pi-specific considerations
- Cross-compilation
- Platform detection
- Binary compatibility

### Image Tagging Strategy

**NEEDS TO BE COMPLETED** - Document:
- Version tags
- Latest tags
- Branch tags
- Release tags
- Tag naming conventions

## Automated Builds

### CI/CD Pipeline

**NEEDS TO BE COMPLETED** - Document:
- GitHub Actions
- Trigger conditions
- Build matrix
- Test integration
- Artifact storage
- Deployment triggers

### Local Build Automation

**NEEDS TO BE COMPLETED** - Scripts for:
- Building all components
- Parallel builds
- Dependency management
- Cleanup scripts
- Version management

## Incremental Builds

**NEEDS TO BE COMPLETED** - Optimize:
- Dependency tracking
- Change detection
- Cache strategy
- Rebuild triggers
- Partial rebuilds

## Build Optimization

### Size Optimization

**NEEDS TO BE COMPLETED** - Techniques:
- Compiler flags
- LTO (Link Time Optimization)
- Dead code elimination
- Unused dependency removal
- Compression

### Speed Optimization

**NEEDS TO BE COMPLETED** - Techniques:
- Parallel compilation
- Distributed compilation
- Cache optimization
- Dependency reduction
- Incremental builds

## Build Configuration

**NEEDS TO BE COMPLETED** - Document:
- Feature flags
- Build profiles (Debug, Release, etc.)
- Target-specific options
- Optimization levels
- Debug symbols

## Build Validation

**NEEDS TO BE COMPLETED** - Procedures:
- Size checks
- Symbol verification
- Dependency validation
- Warning checks
- Error checking

## Build Tools

**NEEDS TO BE COMPLETED** - Document:
- Required tools
- Tool versions
- Tool installation
- Tool configuration
- Path setup

## Troubleshooting Builds

**NEEDS TO BE COMPLETED** - Common issues:
- Out of memory
- Disk space
- Timeout issues
- Compilation errors
- Linking errors
- Version conflicts

## Build Artifacts

**NEEDS TO BE COMPLETED** - Manage:
- Artifact storage
- Artifact cleanup
- Artifact versioning
- Artifact signing
- Artifact distribution

## Performance Benchmarking

**NEEDS TO BE COMPLETED** - Measure:
- Build time
- Binary size
- Compilation stages
- Bottlenecks
- Optimization impact

---

## Related Documentation

- [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md) - Development setup
- [DEBUGGING.md](DEBUGGING.md) - Debugging built binaries
- [TESTING_GUIDE.md](TESTING_GUIDE.md) - Testing built code
- [../02_Hardware_Modules/Firmware/ESP_IDF_Setup.md](../02_Hardware_Modules/Firmware/ESP_IDF_Setup.md) - ESP-IDF details
