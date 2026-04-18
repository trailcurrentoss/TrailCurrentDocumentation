# PlatformIO Setup (Legacy / Spotter-only)

> **Status (2026-04):** All TrailCurrent hardware modules have been migrated to
> **ESP-IDF native**. The only remaining PlatformIO consumer in the platform is
> the [Spotter](../Spotter.md) in-vehicle trailer monitor display, which wraps
> ESP-IDF through PlatformIO's `espidf` framework for LVGL tooling convenience.
>
> For all other modules, follow [ESP_IDF_Setup.md](./ESP_IDF_Setup.md) instead.
> This document is retained because setting up PlatformIO for Spotter is still
> the supported path for that one project, and because earlier revisions of
> several modules used PlatformIO + Arduino — if you find an old branch, these
> instructions still apply.

## Overview

PlatformIO is a cross-platform build system and library manager for embedded development. Historically TrailCurrent used it with the Arduino framework on ESP32 / ESP32-S3 targets; today only Spotter uses it, and it wraps ESP-IDF underneath rather than Arduino.

## Installation

### Prerequisites

- Linux, macOS, or Windows
- Python 3.7+
- Git
- VS Code (recommended)

### Quick Install (VS Code Extension)

1. Open VS Code
2. Go to Extensions (Ctrl+Shift+X)
3. Search for "PlatformIO IDE"
4. Install the extension
5. Restart VS Code

### CLI Install (Alternative)

```bash
pip install platformio

# Verify installation
pio --version
```

## Project Structure

TrailCurrent PlatformIO modules follow this structure:

```
TrailCurrentPicket/
├── src/
│   ├── main.cpp              # Application entry point
│   ├── globals.h             # GPIO pins, debug macros, constants
│   └── canHelper.h           # CAN message encoding/decoding
├── platformio.ini            # Build configuration and dependencies
├── EDA/                      # KiCAD schematics (optional)
├── CAD/                      # FreeCAD enclosure designs (optional)
└── README.md
```

## Configuration (platformio.ini)

Each module's `platformio.ini` defines the target board, framework, and library dependencies. Example:

```ini
[env:esp32c6]
platform = espressif32
board = esp32-c6-devkitm-1
framework = arduino
monitor_speed = 115200
lib_deps =
    https://github.com/YourOrg/Esp32C6TwaiTaskBasedLibrary.git
    https://github.com/YourOrg/Esp32C6OtaUpdateLibrary.git
    https://github.com/YourOrg/C6SuperMiniRgbLedLibrary.git
```

### Common Board Targets

| Board ID | ESP32 Variant | Used By |
|----------|--------------|---------|
| `esp32dev` | ESP32 (WROOM) | Torrent, Tapper |
| `esp32-c6-devkitm-1` | ESP32-C6 (Waveshare C6-Zero) | Aftline |
| `esp32s3zero` | ESP32-S3 (S3-Zero) | Plateau, Borealis |
| `esp32-s3-devkitm-1` | ESP32-S3 (Waveshare Relay-6CH) | Switchback |

## Building a Module

### 1. Clone and Open

```bash
git clone <module-repo-url>
cd TrailCurrentPicket
code .   # Opens in VS Code with PlatformIO
```

PlatformIO auto-detects `platformio.ini` and resolves all dependencies on first build.

### 2. Build

```bash
pio run
```

Or use the PlatformIO toolbar in VS Code (checkmark icon).

### 3. Flash to Device

```bash
# Auto-detect serial port
pio run -t upload

# Specify port
pio run -t upload --upload-port /dev/ttyUSB0
```

### 4. Monitor Serial Output

```bash
pio device monitor

# Or with baud rate
pio device monitor -b 115200
```

Exit with `Ctrl+C`.

### 5. Build + Flash + Monitor (Combined)

```bash
pio run -t upload && pio device monitor
```

## Shared Libraries

TrailCurrent uses several shared libraries across modules, pulled in via `lib_deps` in `platformio.ini`:

### ESP32-C6 Libraries
- **Esp32C6TwaiTaskBasedLibrary** — Task-based CAN (TWAI) driver for ESP32-C6
- **Esp32C6OtaUpdateLibrary** — OTA firmware update support for ESP32-C6
- **C6SuperMiniRgbLedLibrary** — RGB LED driver for ESP32-C6 SuperMini boards

### ESP32 (WROOM) Libraries
- **TwaiTaskBasedLibraryWROOM32** — Task-based CAN (TWAI) driver for ESP32 WROOM
- **OtaUpdateLibraryWROOM32** — OTA firmware update support for ESP32 WROOM

### Common Libraries
- **ESP32ArduinoDebugLibrary** — Compile-time debug macro system (all modules)

## CAN Bus Configuration

All modules use the ESP32's built-in TWAI controller with an external SN65HVD230 transceiver. Standard configuration:

- **Bitrate**: 500 kbps
- **TX Pin**: Defined in `globals.h` (varies per module)
- **RX Pin**: Defined in `globals.h` (varies per module)

CAN message encoding/decoding is typically handled in `canHelper.h` within each module.

## OTA Updates

Modules support over-the-air firmware updates triggered via CAN message (ID 0x00). When the module receives an OTA notification matching its MAC address, it:

1. Pauses normal operation
2. Connects to WiFi (credentials provisioned via CAN)
3. Downloads the new firmware from the update server
4. Flashes and reboots

## Troubleshooting

### "PlatformIO: command not found"

```bash
# Ensure platformio is in PATH
export PATH="$PATH:$HOME/.platformio/penv/bin"
```

### "Device or resource busy"

Serial port already in use:
```bash
lsof /dev/ttyUSB0
kill -9 [pid]
```

### Library dependency errors

Force a clean rebuild:
```bash
pio run -t clean
pio run
```

### Board not detected

Check USB connection and drivers:
```bash
ls /dev/ttyUSB*    # Linux
ls /dev/cu.*       # macOS
```

For ESP32-C6 boards, you may need to hold the BOOT button while connecting.

## Best Practices

1. **Pin definitions in globals.h** — Keep all GPIO assignments in one file
2. **CAN encoding in canHelper.h** — Centralize message packing/unpacking
3. **Use shared libraries** — Don't duplicate TWAI or OTA code
4. **Test locally before OTA** — Flash via USB first, OTA for field updates
5. **Match DBC definitions** — Ensure CAN message formats match `TrailCurrent.dbc`

## Links & Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [Arduino-ESP32 Documentation](https://docs.espressif.com/projects/arduino-esp32/)
- [ESP32 Arduino Core GitHub](https://github.com/espressif/arduino-esp32)

---

See also:
- [02_Hardware_Modules/README.md](../README.md) - Hardware modules overview
- [ESP_IDF_Setup.md](./ESP_IDF_Setup.md) - ESP-IDF setup (for Fireside module)
- [07_Development/BUILD_SYSTEM.md](../../07_Development/BUILD_SYSTEM.md) - Build procedures
- [10_Reference/CAN_BUS_REFERENCE.md](../../10_Reference/CAN_BUS_REFERENCE.md) - CAN message definitions
