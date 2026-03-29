# TrailCurrent Hardware Modules

Complete documentation for all ESP32-based hardware modules in the TrailCurrent platform.

## Overview

Hardware modules are ESP32 microcontroller boards that handle specific functions in the TrailCurrent system. They communicate with each other and the vehicle compute system via the CAN bus.

## Module Categories

Hardware modules are organized into four functional categories:

### 1. **Sensor Modules** - Data Collection
Modules that read environmental and operational data

- [Bearing](./Bearing.md) - GNSS location, heading, altitude, and precise timing
- [Borealis](./Borealis.md) - Temperature, humidity, CO2, and indoor air quality monitoring
- [Picket](./Picket.md) - Cabinet and door open/closed status monitoring
- [Ampline](./Ampline.md) - Power consumption tracking and state-of-charge via Victron Shunt
- [Plateau](./Plateau.md) - Tilt/level measurement on both axes

### 2. **Control Modules** - Action Execution
Modules that control equipment and systems

- [Torrent](./Torrent.md) - 8-channel smart power delivery module with on/off switching and PWM dimming
- [Therma](./Therma.md) - Dual-relay automatic heating/cooling to maintain set temperature
- [Solstice](./Solstice.md) - Victron MPPT solar charge controller interface
- [Switchback](./Switchback.md) - 6-channel relay module for switching high-current loads (up to 3 modules per bus)

### 3. **Communication/Gateway Modules** - Integration
Modules that connect external devices or provide communication bridges

- [Aftline](./Aftline.md) - Trailer wiring harness monitor (all 7 pins: signals, lights, connection status)
- RV-C Gateway *(Coming Soon)* - RV-C protocol gateway for industry-standard device integration

### 4. **User Interface Modules** - Interaction
Modules that allow user control and status display

- [Tapper](./Tapper.md) - Physical 8-button interface for Torrent commands
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
0x00:       OTA Update Notification (PiCanToMqtt)
0x06-0x09:  Bearing (GNSS) - DateTime, Speed/Course, Altitude, Lat/Lon
0x0A-0x11:  Picket - Door/cabinet sensor status (8 addressable modules)
0x15:       Brightness Control (BtGateway → Torrent)
0x18:       Toggle On/Off Command (Tapper/BtGateway → Torrent)
0x1B:       Device Status Report (Torrent → all)
0x1E:       Light Sequence Command (BtGateway → Torrent)
0x1F:       Environment Sensor Data (Borealis)
0x20:       Leveling Config (PiCanToMqtt → Plateau)
0x23-0x24:  Battery Shunt Data (Ampline)
0x25-0x27:  Switchback Toggle Commands (3 module instances)
0x28-0x2A:  Switchback Status Reports (3 module instances)
0x2C-0x2D:  Solar MPPT Data (Solstice)
0x30-0x32:  Vehicle Leveler Data (Plateau) - Tilt, Corners, Status
```

(See [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) for full details)

## Development & Firmware

### Firmware Development
- **Primary Framework**: PlatformIO with Arduino framework (most modules)
- **Alternative Framework**: ESP-IDF (Fireside wireless display)
- **Language**: C/C++
- **IDE**: VS Code + PlatformIO extension (recommended)
- **Build System**: PlatformIO CLI (`pio run`, `pio run -t upload`, `pio device monitor`)
- **Version Control**: Git (one repo per module)

### Setup for Hardware Development

**PlatformIO modules (most modules):**
1. Install PlatformIO (VS Code extension or CLI)
2. Clone the module repository
3. PlatformIO auto-resolves dependencies from `platformio.ini`
4. Build with `pio run`
5. Flash with `pio run -t upload`
6. Monitor with `pio device monitor`

**ESP-IDF modules (Fireside):**
1. Install ESP-IDF
2. Clone the module repository
3. Build with `idf.py build`
4. Flash with `idf.py flash`
5. Monitor with `idf.py monitor`

See [Firmware/PlatformIO_Setup.md](./Firmware/PlatformIO_Setup.md) for PlatformIO setup or [Firmware/ESP_IDF_Setup.md](./Firmware/ESP_IDF_Setup.md) for ESP-IDF setup.

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
├─ Requires: CAN bus, dual relay output
├─ Depends on: Borealis temperature feedback
└─ Outputs: Heating/cooling relay control

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

Three ESP32 variants are used across the platform:

| Variant | Board | Used By |
|---------|-------|---------|
| ESP32 (WROOM) | Various dev boards | Ampline, Torrent, Tapper, Solstice, BtGateway, CanEspNowGateway |
| ESP32-C6 | Waveshare ESP32-C6-Zero | Aftline, Therma |
| ESP32-S3 | Waveshare ESP32-S3-RS485-CAN | Picket, Solstice |
| ESP32-S3 | S3-Zero / Waveshare S3-Relay-6CH | Plateau, Switchback, Borealis |

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

- `TrailCurrentGnssModule/` - Bearing (GNSS)
- `TrailCurrentBorealis/` - Borealis (air quality, temp, humidity, CO2)
- `TrailCurrentPicket/` - Picket (cabinet & door sensors)
- `TrailCurrentAmpline/` - Ampline (shunt interface)
- `TrailCurrentPlateau/` - Plateau (vehicle level sensor)
- `TrailCurrentTorrent/` - Torrent (power delivery module)
- `TrailCurrentTherma/` - Therma (climate relay controller)
- `TrailCurrentSolstice/` - Solstice (MPPT solar controller interface)
- `TrailCurrentAftline/` - Aftline (trailer wiring harness monitor)
- `TrailCurrentTapper/` - Tapper (8-button panel)
- `TrailCurrentFireside/` - Fireside (wireless touchscreen display)
- `TrailCurrentMilepost/` - Milepost (hardwired CAN bus touchscreen)
- `TrailCurrentSpotter/` - Spotter (in-vehicle trailer monitor display)
- `TrailCurrentPeregrine/` - Peregrine (AI voice assistant)
- `TrailCurrentSwitchback/` - Switchback (6-channel relay module)

---

## See Also

- [MODULE_CATEGORIES.md](MODULE_CATEGORIES.md) - Organized by function
- [Firmware/ESP_IDF_Setup.md](Firmware/ESP_IDF_Setup.md) - Setting up development
- [07_Development/DEVELOPMENT_SETUP.md](../07_Development/DEVELOPMENT_SETUP.md) - Full dev environment
- [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) - CAN message definitions
- [10_Reference/GPIO_PIN_MAPPING.md](../10_Reference/GPIO_PIN_MAPPING.md) - Pin configurations
