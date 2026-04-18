# TrailCurrent Hardware Modules

Complete documentation for all ESP32-based hardware modules in the TrailCurrent platform.

## Overview

Hardware modules are ESP32 microcontroller boards that handle specific functions in the TrailCurrent system. They communicate with each other and the vehicle compute system via the CAN bus.

## Module Categories

Hardware modules are organized into four functional categories:

### 1. **Sensor Modules** - Data Collection
Modules that read environmental and operational data

- [Bearing](./Bearing.md) - GNSS location, heading, altitude, and precise timing
- [Borealis](./Borealis.md) - Temperature, humidity, TVOC, and eCO2 air quality monitoring (SHT31-D + SGP30)
- [Picket](./Picket.md) - Cabinet and door open/closed status monitoring (up to 8 modules per bus, up to 13 reed switch inputs each)
- [Plateau](./Plateau.md) - Tilt/level measurement on both axes (BNO055 IMU)
- [Reservoir](./Reservoir.md) - Water tank level monitoring for up to 3 tanks (fresh, grey, black) via contactless sensors

### 2. **Control Modules** - Action Execution
Modules that control equipment and systems

- [Torrent](./Torrent.md) - 8-channel smart power delivery module with on/off switching and PWM dimming (up to 3 modules per bus)
- [Therma](./Therma.md) - Closed-loop thermostat: 3-board system (controller + heater relay + cooler relay) with mutually-exclusive heat/cool outputs and configurable hysteresis
- [Solstice](./Solstice.md) - Victron MPPT solar + SmartShunt battery gateway (state-of-charge, power consumption, solar generation)
- [Switchback](./Switchback.md) - 6-channel relay module for switching high-current loads (up to 3 modules per bus)

### 3. **Communication/Gateway Modules** - Integration
Modules that connect external devices or provide communication bridges

- [Aftline](./Aftline.md) - Trailer wiring harness monitor (all 7 pins: signals, lights, connection status)
- RV-C Gateway *(Coming Soon)* - RV-C protocol gateway for industry-standard device integration

### 4. **User Interface Modules** - Interaction
Modules that allow user control and status display

- [Tapper](./Tapper.md) - Physical 8-button interface for Torrent or Switchback commands
- [Fireside](./Fireside.md) - Wireless battery-powered touchscreen display with wall cradle
- [Milepost](./Milepost.md) - Hardwired CAN bus touchscreen (always-on, multiple locations)
- [Spotter](./Spotter.md) - In-vehicle display that monitors trailer status while towing

### 5. **Voice & AI Modules**

- [Peregrine](./Peregrine.md) - AI voice assistant with system access and hands-free control

## Module Statistics

| Category | Modules | Primary Function |
|----------|---------|------------------|
| Sensors | 5 | Data collection |
| Control | 4 | System control |
| Gateway | 2 | Device integration (1 coming soon) |
| Interface | 4 | User interaction |
| Voice & AI | 1 | Intelligent assistance |
| **Total** | **16** | - |

## Communication Protocol

All hardware modules communicate using **CAN Bus (Controller Area Network)**:

- **Standard**: ISO 11898-1 (CAN 2.0A)
- **Speed**: 500 kbps
- **Isolation**: Galvanically isolated from other networks
- **Range**: 40m at 500 kbps
- **Reliability**: High noise immunity, automatic error detection

### CAN IDs by Module

```
0x00:       OTA Update Notification (Headwaters)
0x01:       WiFi Config Provisioning (Headwaters)
0x02:       Discovery Trigger (Headwaters)
0x03:       Discovery Reset (Headwaters)
0x04:       Firmware Version Report (any module, event-driven at boot)
0x06-0x09:  Bearing (GNSS) - DateTime, Sat/Speed/Course, Altitude, Lat/Lon
0x0A-0x11:  Picket - Door/cabinet sensor status (8 addressable modules)
0x15-0x17:  Torrent Brightness Control (per-address, events from any sender)
0x18-0x1A:  Torrent Toggle On/Off Commands (per-address)
0x1B-0x1D:  Torrent Device Status Reports (per-address)
0x1F:       Environment Sensor Data (Borealis)
0x20:       Leveling Config (Headwaters → Plateau)
0x21:       Borealis Calibration (Headwaters → Borealis)
0x23-0x24:  SmartShunt Basic Data (Solstice — battery V/A/SOC/W/TTG from VE.Direct TEXT)
0x25-0x27:  Switchback Toggle Commands (3 module instances)
0x28-0x2A:  Switchback Status Reports (3 module instances)
0x2B:       Shunt Extended Live (Solstice — reserved, transmits zeros until HEX GET TX wire)
0x2C-0x2D:  Solar MPPT Data (Solstice)
0x2E:       Solar Load Control (any sender → Solstice)
0x2F:       Shunt Extended History (Solstice — reserved, transmits zeros until HEX GET TX wire)
0x30-0x32:  Vehicle Leveler Data (Plateau) - Tilt, Corners, Status
0x33-0x35:  Torrent Light Sequence Commands (per-address)
0x3E:       Water Tank Levels (Reservoir)
0x3F:       Therma Desired Temperature
0x40:       Therma Status
0x41:       Therma Set Desired Temperature Request (any sender → Therma)
0x42:       Therma Set Threshold Request (any sender → Therma)
```

(See [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) for full details — the authoritative source is [TrailCurrent.dbc](../TrailCurrent.dbc))

## Development & Firmware

### Firmware Development
- **Primary Framework**: ESP-IDF (all ESP32 hardware modules)
- **Version**: ESP-IDF v5.1+ — most modules track v5.5
- **Language**: C/C++
- **IDE**: VS Code + ESP-IDF extension (recommended)
- **Build System**: ESP-IDF CLI (`idf.py build`, `idf.py flash`, `idf.py monitor`)
- **Version Control**: Git (one repo per module)

All hardware modules in this directory are built with ESP-IDF. Earlier revisions of
several modules used PlatformIO with the Arduino framework; those have all been
migrated. The only remaining PlatformIO consumer in the platform is the
[Spotter](./Spotter.md) in-vehicle display, which wraps ESP-IDF through PlatformIO's
`espidf` framework for LVGL tooling convenience.

### Setup for Hardware Development

1. Install ESP-IDF (v5.1 or later — most modules track v5.5)
2. Clone the module repository
3. Source the environment: `. $HOME/esp/esp-idf/export.sh`
4. Build with `idf.py build`
5. Flash with `idf.py flash`
6. Monitor with `idf.py monitor`

See [Firmware/ESP_IDF_Setup.md](./Firmware/ESP_IDF_Setup.md) for detailed setup.

### Multi-Instance Modules

Some modules support multiple physical units on the same CAN bus. Each instance is built with a unique address at compile time, producing a separate firmware binary per address. These modules include a `build-all.sh` script to build all variants at once.

| Module | Instances | Binaries per Release |
|--------|-----------|---------------------|
| Torrent | 3 (addr 0-2) | `torrent_addr0.bin` .. `torrent_addr2.bin` |
| Switchback | 3 (addr 0-2) | `switchback_addr0.bin` .. `switchback_addr2.bin` |
| Picket | 8 (addr 0-7) | `picket_addr0.bin` .. `picket_addr7.bin` |
| Tapper | 6 (2 targets × 3 addr) | `tapper_torrent_addr0.bin` .. `tapper_switchback_addr2.bin` |
| Therma | 3 (controller + heater relay + cooler relay) | `therma_controller.bin`, `therma_heater_relay.bin`, `therma_cooler_relay.bin` (each also published as `*_merged.bin` for the web flasher) |

See [07_Development/MULTI_INSTANCE_MODULES.md](../07_Development/MULTI_INSTANCE_MODULES.md) for the full pattern and implementation details.

## Key Features Across Modules

### Shared Capabilities

All modules typically include:

- **CAN Communication**: Core protocol for inter-module communication
- **Status LED**: RGB LED for visual status indication
- **OTA Updates**: Firmware updates over CAN bus or WiFi
- **Deep Sleep**: Power management for battery operation
- **Configuration**: Stored in NVS (Non-Volatile Storage)
- **Logging**: Debug logging via UART or over CAN

### Optional Features

- **Bluetooth**: Direct mobile app communication (some modules)
- **WiFi**: Direct cloud communication (some modules)
- **SD Card**: Local data logging (some modules)
- **Display**: Built-in UI (interface modules)
- **Buttons**: Physical control (interface modules)

## Module Dependencies

```
Bearing (GNSS)
├─ Requires: CAN bus
├─ Optional: UART for serial input
└─ Outputs: Position, heading, altitude, timing data to CAN

Borealis (Air Quality)
├─ Requires: CAN bus, I2C or SPI sensors
└─ Outputs: Temperature, humidity, CO2 readings to CAN

Torrent (Power Delivery)
├─ Requires: CAN bus, relay/MOSFET outputs
├─ Depends on: Multiple sensors for logic
└─ Outputs: Power state commands to 8 channels

Therma (Climate Control)
├─ Requires: CAN bus (controller only), 2× ESP32-S3-Relay-1CH boards driven over GPIO
├─ Depends on: Borealis temperature feedback (CAN 0x1F)
└─ Outputs: ThermaDesiredTemperature (0x3F), ThermaStatus (0x40); accepts setpoint/threshold change requests (0x41/0x42) from any device

Switchback (Relay Control)
├─ Requires: CAN bus, 6 relay outputs
├─ Depends on: Toggle commands from BtGateway/PiCanToMqtt
└─ Outputs: Relay state bitmask to CAN

User Interface (Tapper, Fireside, Milepost, Spotter)
├─ Requires: CAN bus, buttons/display
├─ Depends on: Status from other modules
└─ Outputs: User commands to CAN
```

## Hardware Specifications

### Common Hardware

Several ESP32 variants are used across the platform:

| Variant | Board | Used By |
|---------|-------|---------|
| ESP32 (WROOM) | Various dev boards | Torrent, Tapper |
| ESP32-S3 | Waveshare ESP32-S3-RS485-CAN | Bearing, Solstice, Picket, Aftline, Therma controller, Reservoir, Milepost (custom LCD variant) |
| ESP32-S3 | Waveshare ESP32-S3-Relay-1CH | Therma heater/cooler relay boards |
| ESP32-S3 | Waveshare ESP32-S3-ETH-8DI-8RO-C | Switchback |
| ESP32-S3 | S3-Zero | Plateau, Borealis |
| ESP32-S3 | Waveshare ESP32-S3-Touch-LCD-7 | Milepost (7" display) |
| ESP32-P4 | Waveshare ESP32-P4-WiFi6-Touch-LCD-7B | Fireside |

- **Operating Voltage**: 3.3V (internal), 5-24V (input with regulator)
- **Power Consumption**: 80mA typical, 10µA deep sleep
- **Storage**: 4MB Flash (variable)
- **RAM**: 320KB (variable per variant)
- **GPIO**: 25+ (variable per module)

### CAN Interface
- **Controller**: ESP32 built-in TWAI (Two-Wire Automotive Interface)
- **Transceiver**: SN65HVD230 (external, converts TWAI signals to CAN bus levels)
- **Speed**: 500 kbps
- **Termination**: 120Ω resistors at both ends of bus

See [10_Reference/HARDWARE_SPECIFICATIONS.md](../10_Reference/HARDWARE_SPECIFICATIONS.md) for detailed specs per module.

## Testing & Validation

### Unit Testing
- Tested during development on breadboard/prototype
- Uses PlatformIO or Arduino IDE for local testing

### Integration Testing
- Tested on vehicle with full CAN bus
- Validates message reception and transmission
- Checks interaction with other modules

### System Testing
- Full platform tests with all modules
- Cloud integration tests
- User interface tests

## Troubleshooting

Common issues and solutions:

| Issue | Cause | Solution |
|-------|-------|----------|
| Module not on CAN bus | CAN ID conflict or connection issue | Check CAN wiring, verify ID in firmware |
| CAN messages not received | Incorrect baud rate | Configure CAN speed to match |
| Module reboots | Watchdog timeout or crash | Check logs, validate loop timing |
| Power consumption too high | Sleep mode not enabled | Configure sleep/wake timers |
| Firmware flash failure | Connection issue or corrupt binary | Erase chip, re-flash carefully |

See [09_Troubleshooting/](../09_Troubleshooting/) for more detailed troubleshooting guides.

## Adding a New Module

To create a new hardware module:

1. Clone an existing module repo as template
2. Modify CAN IDs (choose from available ranges)
3. Update hardware pins for your circuit
4. Implement required message handlers
5. Test locally on breadboard
6. Integrate and test on vehicle
7. Document in this folder

See [07_Development/CONTRIBUTING.md](../07_Development/CONTRIBUTING.md) for contribution guidelines.

## Performance Benchmarks

| Metric | Typical Value |
|--------|---------------|
| CAN Message Latency | <10ms |
| Sensor Read Latency | 1-100ms (sensor dependent) |
| Module Startup Time | 2-5 seconds |
| Firmware Boot Time | <2 seconds |
| CAN Bus Utilization | 5-20% typical |
| Memory Usage | 60-80% of 320KB RAM |

## Links to Source Code

All module source code is in `/Product/`:

- `TrailCurrentBearing/` - Bearing (GNSS)
- `TrailCurrentBorealis/` - Borealis (air quality, temp, humidity, TVOC, eCO2)
- `TrailCurrentPicket/` - Picket (cabinet & door sensors)
- `TrailCurrentPlateau/` - Plateau (vehicle level sensor)
- `TrailCurrentTorrent/` - Torrent (power delivery module)
- `TrailCurrentTherma/` - Therma (closed-loop thermostat: controller + heater/cooler relay boards)
- `TrailCurrentSolstice/` - Solstice (MPPT solar controller + SmartShunt battery gateway)
- `TrailCurrentAftline/` - Aftline (trailer wiring harness monitor)
- `TrailCurrentReservoir/` - Reservoir (water tank level monitor)
- `TrailCurrentTapper/` - Tapper (8-button panel)
- `TrailCurrentFireside/` - Fireside (ESP32-P4 touchscreen display, wireless)
- `TrailCurrentMilepost/` - Milepost (hardwired CAN bus touchscreen)
- `TrailCurrentSpotter/` - Spotter (in-vehicle trailer monitor display)
- `TrailCurrentPeregrine/` - Peregrine (AI voice assistant, Radxa Dragon Q6A)
- `TrailCurrentSwitchback/` - Switchback (8-channel relay module)

---

## See Also

- [MODULE_CATEGORIES.md](MODULE_CATEGORIES.md) - Organized by function
- [Firmware/ESP_IDF_Setup.md](Firmware/ESP_IDF_Setup.md) - Setting up development
- [07_Development/DEVELOPMENT_SETUP.md](../07_Development/DEVELOPMENT_SETUP.md) - Full dev environment
- [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) - CAN message definitions
- [10_Reference/GPIO_PIN_MAPPING.md](../10_Reference/GPIO_PIN_MAPPING.md) - Pin configurations
