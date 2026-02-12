# TrailCurrent Shared Libraries

Documentation for the shared libraries used across TrailCurrent ESP32 modules.

## Overview

Shared libraries provide common functionality to multiple hardware modules, reducing code duplication and ensuring consistency across the platform.

## Available Libraries

### Debug Library
**Purpose**: Unified debugging and logging across all modules

**Features**:
- Serial UART logging
- CAN bus logging
- Log level filtering
- Timestamped output
- Circular buffer for recent logs

**Source**: `/Product/Libraries/DebugLibrary/`

**Used By**: All modules

### OTA Library (Standard)
**Purpose**: Over-the-air firmware updates for WROOM32 modules

**Features**:
- Firmware binary reception via CAN bus
- Flash memory management
- Rollback on failed update
- Update progress tracking
- Automatic reboot after update

**Source**: `/Product/Libraries/OtaLibrary/`

**Used By**: Most modules with standard ESP32

### OTA Library (WROOM32)
**Purpose**: OTA updates specifically for WROOM32 variants

**Features**:
- WROOM32-specific memory layout
- Dual partition support
- Enhanced stability
- Factory reset capability

**Source**: `/Product/Libraries/OtaUpdateLibraryWROOM32/`

**Used By**: WROOM32-based modules

### RGB LED Library
**Purpose**: Unified RGB LED status indication

**Features**:
- Color definitions
- Blinking patterns
- Fade effects
- Status indication modes
- Low-level GPIO control

**Source**: `/Product/Libraries/RgbLedLibrary/`

**Used By**: Modules with status LEDs

### TWAI CAN Library (Standard)
**Purpose**: CAN communication using ESP32 TWAI peripheral

**Features**:
- Hardware CAN interface
- Message filtering
- Transmission queuing
- Interrupt-driven reception
- Error handling and diagnostics

**Source**: `/Product/Libraries/TwaiTaskBased/`

**Used By**: Modules without external CAN transceiver

### TWAI CAN Library (WROOM32)
**Purpose**: TWAI CAN for WROOM32-specific implementations

**Features**:
- WROOM32 hardware optimization
- Task-based architecture
- Enhanced throughput
- Memory-efficient buffering

**Source**: `/Product/Libraries/TwaiTaskBasedLibraryWROOM32/`

**Used By**: WROOM32 modules using built-in CAN

## Library Structure

Each library follows a standard structure:

```
LibraryName/
├── CMakeLists.txt           # Build configuration
├── include/                 # Header files (.h)
├── src/                     # Source files (.c)
├── README.md               # Library documentation
└── example/                # Example usage (optional)
```

## Using Libraries

### Adding to a Module Project

1. Add library as component in `CMakeLists.txt`:
```cmake
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES debug_library twai_library rgb_led_library
)
```

2. Include header in source:
```c
#include "debug_library.h"
#include "twai_library.h"
```

3. Initialize in code:
```c
debug_init();
twai_init();
```

### Configuration

Libraries are configured via:
- `sdkconfig` (menuconfig)
- Header defines
- Runtime configuration functions

See individual library documentation for configuration options.

## API Reference

### Debug Library
```c
void debug_init(void);
void debug_log(const char *format, ...);
void debug_log_error(const char *format, ...);
void debug_log_raw(const char *data, int len);
```

### OTA Library
```c
void ota_init(void);
void ota_start_update(uint32_t image_size);
void ota_write_chunk(const uint8_t *data, size_t len);
void ota_complete_update(void);
void ota_rollback(void);
```

### RGB LED Library
```c
void led_init(int r_gpio, int g_gpio, int b_gpio);
void led_set_color(uint8_t r, uint8_t g, uint8_t b);
void led_blink(uint32_t on_ms, uint32_t off_ms);
void led_pulse(uint32_t period_ms);
```

### TWAI CAN Library
```c
void twai_init(int tx_io, int rx_io, uint32_t baudrate);
void twai_send_frame(twai_message_t *msg);
bool twai_recv_frame(twai_message_t *msg, int timeout_ms);
void twai_deinit(void);
```

## Library Dependencies

```
All Modules
    ├─ Debug Library (for logging)
    ├─ TWAI Library OR external CAN driver
    ├─ OTA Library (for updates)
    ├─ RGB LED Library (optional, for status)
    └─ ESP-IDF components (HAL, driver, etc.)
```

## Building Libraries

Libraries are built as part of the ESP-IDF build system:

```bash
idf.py build
```

Libraries are compiled into the application binary.

## Development & Modification

### When to Modify a Library

- Adding new functionality needed by multiple modules
- Bug fixes
- Performance improvements
- Hardware compatibility updates

### Testing Library Changes

1. Modify library source in `/Product/Libraries/`
2. Test in one module first
3. Validate with multiple modules
4. Update documentation
5. Commit changes

### Adding New Library

1. Create directory in `/Product/Libraries/NewLibrary/`
2. Create CMakeLists.txt
3. Add include/ and src/ directories
4. Create README.md
5. Test integration

## Performance Characteristics

| Library | Overhead | Notes |
|---------|----------|-------|
| Debug | Minimal | Disabled in production |
| OTA | 10s update time | Depends on image size |
| RGB LED | <1ms | Simple GPIO writes |
| TWAI CAN | <1ms per frame | Hardware interrupt driven |

## Version Management

Libraries are versioned along with modules:
- Semantic versioning (MAJOR.MINOR.PATCH)
- Version stored in library header
- Check compatibility with module requirements

## Documentation

Each library has:
- README.md with overview
- API documentation in headers
- Example code
- Configuration guide

## Source Code

Shared libraries location: `/Product/Libraries/`

---

See also:
- [02_Hardware_Modules/](../02_Hardware_Modules/) - Hardware modules
- [07_Development/DEVELOPMENT_SETUP.md](../07_Development/DEVELOPMENT_SETUP.md) - Development setup
- [02_Hardware_Modules/Firmware/ESP_IDF_Setup.md](../02_Hardware_Modules/Firmware/ESP_IDF_Setup.md) - ESP-IDF details
