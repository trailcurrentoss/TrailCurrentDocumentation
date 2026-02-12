# ESP-IDF Setup and Configuration

Guide to setting up the Espressif IoT Development Framework for TrailCurrent hardware module development.

## Overview

ESP-IDF is the official development framework for ESP32 modules. All TrailCurrent hardware modules use ESP-IDF for firmware development.

## Installation

### Prerequisites

- Linux, macOS, or Windows (WSL)
- Python 3.7+
- Git
- 2GB+ free disk space

### Quick Install

```bash
# Clone ESP-IDF repository
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf

# Install tools
./install.sh esp32
source ./export.sh

# Verify installation
idf.py --version
```

### Add to PATH

Add to your shell profile (~/.bashrc, ~/.zshrc, etc.):

```bash
export PATH="$PATH:$HOME/esp/esp-idf/tools"
alias idf.py='$HOME/esp/esp-idf/tools/idf.py'
source $HOME/esp/esp-idf/export.sh
```

## Project Structure

TrailCurrent modules follow ESP-IDF project structure:

```
TrailCurrentGpsModule/
├── main/
│   ├── main.c                    # Application entry point
│   ├── CMakeLists.txt           # Build configuration
│   └── Kconfig.projbuild        # Configuration options
├── components/                   # Shared components
│   ├── debug_library/
│   ├── twai_library/
│   └── oта_library/
├── CMakeLists.txt               # Project build config
├── sdkconfig                    # Build configuration (generated)
├── README.md
└── CLAUDE.md                    # Development notes
```

## Building a Module

### 1. Set Target

```bash
cd /Product/TrailCurrentGpsModule
idf.py set-target esp32  # or esp32s3
```

### 2. Configure Project

```bash
idf.py menuconfig
```

This opens interactive configuration menu:
- Device configuration
- Component options
- Debug settings
- Performance tuning

### 3. Build

```bash
idf.py build
```

Produces binary in `build/[module].bin`

### 4. Flash to Device

**USB Connection**:
```bash
# Find serial port
ls /dev/ttyUSB*           # Linux
ls /dev/cu.usbserial*     # macOS

# Flash
idf.py -p /dev/ttyUSB0 flash
```

### 5. Monitor Serial Output

```bash
idf.py monitor
# Or:
idf.py -p /dev/ttyUSB0 monitor -b 115200
```

Exit with `Ctrl+]`

## Configuration (menuconfig)

### Device Configuration
```
Component config > ESP32-specific
  - Internal Memory
  - CPU Frequency
  - Hardware Sleep Settings
```

### CAN/TWAI Configuration
```
Component config > CAN
  - CAN ISR Flags
  - Enabled CAN Module
  - RX Queue Depth
  - TX Queue Depth
```

### Debug Configuration
```
Component config > Log Output
  - Default Log Level (set to INFO for development)
  - Enable Serial Output
```

### Common Configurations

**For CAN Modules**:
```
- Enable TWAI
- TWAI frequency: 500 kbps
- RX queue: 32 messages
- TX queue: 16 messages
```

**For Sensors**:
```
- Enable I2C or SPI as needed
- Set clock frequency
- Enable DMA if supported
```

**For WiFi/BT**:
```
- WiFi frequency: 2.4GHz
- Bluetooth: Classic or BLE
- Power saving: Enabled
```

## Build System (CMake)

### Project CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.5)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(trailcurrent_gps)
```

### Component CMakeLists.txt

```cmake
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES debug_library twai_library
)
```

## Including Libraries

### Add Shared Library as Component

1. Copy library path to components:
```bash
cp -r /Product/Libraries/TwaiTaskBased components/
```

2. Reference in main CMakeLists.txt:
```cmake
idf_component_register(REQUIRES twai_library)
```

3. Include in code:
```c
#include "twai_library.h"
```

## OTA Update Support

### Enable OTA

In `sdkconfig` or menuconfig:
```
Component config > App update
  - Enable OTA
  - Configure OTA partition
```

### OTA Code Example

```c
#include "esp_ota_ops.h"

void start_ota_update(const uint8_t *data, size_t len) {
    const esp_partition_t *update_partition =
        esp_ota_get_next_update_partition(NULL);

    esp_ota_begin(update_partition, len, &handle);
    esp_ota_write(handle, data, len);
    esp_ota_end(handle);
    esp_ota_set_boot_partition(update_partition);
    esp_restart();
}
```

## Performance Optimization

### Increase Clock Speed

```bash
idf.py menuconfig
Component config > ESP32-specific > CPU Frequency > 240 MHz
```

### Optimize for Size

```bash
Component config > Compiler options > Optimization Level > -Os
```

### Enable LTO (Link-Time Optimization)

```bash
Component config > Compiler options > Enable LTO > Yes
```

## Debugging

### GDB Debugging

```bash
# Terminal 1: Start GDB server
idf.py gdb

# Terminal 2: Connect with GDB
xtensa-esp32-elf-gdb
(gdb) target remote :3333
(gdb) monitor reset halt
(gdb) break main
(gdb) continue
```

### Serial Monitor Logging

```bash
idf.py monitor --print-filter=esp_system:V,gpio:V
```

### Core Dump Analysis

Enable in menuconfig, then analyze crashes automatically.

## Troubleshooting

### "idf.py: command not found"

```bash
# Run export script
source ~/esp/esp-idf/export.sh
```

### "Device or resource busy"

Serial port already in use:
```bash
# Kill existing connection
lsof /dev/ttyUSB0
kill -9 [pid]
```

### "BUILD FAILED: cmake not found"

Install cmake:
```bash
pip install cmake
```

### Partition Table Issues

Check partition table:
```bash
idf.py partition-table
```

Regenerate if corrupted:
```bash
idf.py partition-table --write
```

## Best Practices

1. **Always version control** - Check in ESP-IDF version used
2. **Use consistent naming** - Follow ESP-IDF conventions
3. **Enable all warnings** - Catch issues early
4. **Test locally first** - Before deploying OTA
5. **Document configs** - Note any special menuconfig changes
6. **Use components** - For shared code

## Testing

### Unit Testing

Use Unity framework (included with IDF):

```c
// test_file.c
#include "unity.h"

TEST_CASE("test GPS parser", "[gps]") {
    // Test code
    TEST_ASSERT_EQUAL(expected, actual);
}
```

Run tests:
```bash
idf.py test
```

## Links & Resources

- [ESP-IDF Official Docs](https://docs.espressif.com/projects/esp-idf/)
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [Hardware Design Guide](https://www.espressif.com/en/support/documents/technical-documents)

---

See also:
- [02_Hardware_Modules/README.md](../README.md) - Hardware modules overview
- [07_Development/BUILD_SYSTEM.md](../../07_Development/BUILD_SYSTEM.md) - Build procedures
- [07_Development/DEBUGGING.md](../../07_Development/DEBUGGING.md) - Debugging techniques
