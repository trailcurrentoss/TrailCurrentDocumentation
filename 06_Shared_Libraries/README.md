# TrailCurrent Shared Libraries

Documentation for the standalone library repos that ESP32 modules can pull in
alongside the standard ESP-IDF managed components.

## Overview

Most shared functionality (CAN, OTA, NVS, logging, LED, WiFi provisioning)
lives directly inside each module as ESP-IDF components. A small number of
standalone library repos remain for legacy Arduino-framework code and for
OTA/CAN helpers that predate the ESP-IDF migration. New modules should prefer
pulling code from ESP-IDF's component manager (`idf_component.yml`) over
depending on these repos.

## Available Libraries

### ESP32ArduinoDebugLibrary
**Purpose**: Unified debugging and logging helpers for the original Arduino-framework modules.

**Source**: `/Product/ESP32ArduinoDebugLibrary/`

**Status**: Legacy. All current modules log via `ESP_LOGx` macros from ESP-IDF directly.

### OtaUpdateLibraryWROOM32
**Purpose**: OTA update routines for the WROOM32-based modules (Torrent, Tapper).

**Features**:
- Dual-OTA partition support with rollback
- CAN-triggered update initiation
- WiFi-based HTTP binary upload
- NVS-persisted update state

**Source**: `/Product/OtaUpdateLibraryWROOM32/`

**Used By**: Torrent, Tapper (the remaining WROOM32-class modules)

### TwaiTaskBasedLibraryWROOM32
**Purpose**: Task-based TWAI (ESP32 built-in CAN) driver used by WROOM32 modules.

**Features**:
- Dedicated FreeRTOS task for CAN RX
- Transmission queue with back-pressure handling
- Automatic bus-off recovery
- Hardware interrupt driven RX

**Source**: `/Product/TwaiTaskBasedLibraryWROOM32/`

**Used By**: Torrent, Tapper

### Migration Note

Earlier revisions shipped dedicated libraries for the ESP32-C6 SuperMini
boards (`Esp32C6OtaUpdateLibrary`, `Esp32C6TwaiTaskBasedLibrary`,
`C6SuperMiniRgbLedLibrary`). Those have been **retired** because the modules
that used them (initially Aftline and Therma) have moved to the
Waveshare ESP32-S3-RS485-CAN / ESP32-S3-Relay-1CH boards and now consume
standard ESP-IDF drivers directly. If you are maintaining an old branch of
one of those modules, the original libraries remain available in
git history.

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

1. Modify library source in its repo under `/Product/`
2. Test in one module first
3. Validate with multiple modules
4. Update documentation
5. Commit changes

### Adding New Library

1. Create a new repo under `/Product/NewLibrary/`
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

Shared libraries are standalone repos under `/Product/`, each hosted at `github.com/trailcurrentoss/`.

---

See also:
- [02_Hardware_Modules/](../02_Hardware_Modules/) - Hardware modules
- [07_Development/DEVELOPMENT_SETUP.md](../07_Development/DEVELOPMENT_SETUP.md) - Development setup
- [02_Hardware_Modules/Firmware/ESP_IDF_Setup.md](../02_Hardware_Modules/Firmware/ESP_IDF_Setup.md) - ESP-IDF details
