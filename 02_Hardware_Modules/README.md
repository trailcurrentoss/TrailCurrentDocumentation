# TrailCurrent Hardware Modules

Complete documentation for all ESP32-based hardware modules in the TrailCurrent platform.

## Overview

Hardware modules are ESP32 microcontroller boards that handle specific functions in the TrailCurrent system. They communicate with each other and the vehicle compute system via the CAN bus.

## Module Categories

Hardware modules are organized into four functional categories:

### 1. **Sensor Modules** - Data Collection
Modules that read environmental and operational data

- [GPS Module](./GPS_Module.md) - Vehicle location tracking via GPS
- [Temperature Sensor](./Temperature_Sensor.md) - Environmental temperature monitoring
- [Air Quality Module](./Air_Quality_Module.md) - Pollution and particulate measurement
- [Cabinet & Door Sensor](./Cabinet_Door_Sensor.md) - Intrusion/opening detection
- [Shunt Gateway](./Shunt_Gateway.md) - Power consumption and battery monitoring

### 2. **Control Modules** - Action Execution
Modules that control equipment and systems

- [Power Control Module (PCM)](./Power_Control_Module.md) - Main power distribution and management
- [Electric Heater Control](./Electric_Heater_Control.md) - Heater activation and temperature management
- [Vehicle Leveler](./Vehicle_Leveler.md) - Automatic leveling system control
- [MPPT CAN Gateway](./MPPT_CAN_Gateway.md) - Solar charge controller interface

### 3. **Communication/Gateway Modules** - Integration
Modules that connect external devices or provide communication bridges

- [BT Gateway](./BT_Gateway.md) - Bluetooth connectivity for mobile devices
- [CAN/EspNow Gateway](./CAN_EspNow_Gateway.md) - Multi-protocol gateway for extended range
- [Trailer Monitoring Module](./Trailer_Monitoring.md) - 7-pin and other trailer connectors

### 4. **User Interface Modules** - Interaction
Modules that allow user control and status display

- [Eight Button Panel](./Eight_Button_Panel.md) - Physical button control interface
- [Wall-Mounted Display (Standard)](./Wall_Mounted_Display.md) - Standard wall display with buttons
- [Wall-Mounted Display (7" Sunton)](./Wall_Mounted_Display_Sunton.md) - High-resolution 7" display
- [EspNow Remote Control](./EspNow_Remote_Control.md) - Wireless remote control
- [Waveshare ESP32S3 Remote](./Waveshare_ESP32S3_Remote.md) - Specific remote hardware

## Module Statistics

| Category | Modules | Primary Function |
|----------|---------|------------------|
| Sensors | 5 | Data collection |
| Control | 4 | System control |
| Gateway | 3 | Device integration |
| Interface | 5 | User interaction |
| **Total** | **17** | - |

## Communication Protocol

All hardware modules communicate using **CAN Bus (Controller Area Network)**:

- **Standard**: ISO 11898-1
- **Speed**: 500 kbps or 1 Mbps (configured per project)
- **Isolation**: Galvanically isolated from other networks
- **Range**: 40m at 500 kbps, shorter at higher speeds
- **Reliability**: High noise immunity, automatic error detection

### CAN IDs by Module Type

```
0x000-0x0FF: GPS & Location
0x100-0x1FF: Temperature & Environment
0x200-0x2FF: Power & Control (Commands)
0x300-0x3FF: Status & Telemetry
0x400-0x4FF: User Interface
0x500-0x5FF: Gateway/Bridge
0x600-0x6FF: External Systems & Subsystems
0x700-0x7FF: Reserved
0x800-0x8FF: Diagnostic/Debug
```

(See [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) for full details)

## Development & Firmware

### Firmware Development
- **Framework**: ESP-IDF (Espressif IoT Development Framework)
- **Language**: C/C++
- **IDE**: VS Code + ESP-IDF extension (recommended)
- **Build System**: CMake + idf.py
- **Version Control**: Git (one repo per module)

### Setup for Hardware Development

1. Install ESP-IDF
2. Clone the module repository
3. Configure CAN parameters (if needed)
4. Build with `idf.py build`
5. Flash with `idf.py flash`
6. Monitor with `idf.py monitor`

See [Firmware/ESP_IDF_Setup.md](./Firmware/ESP_IDF_Setup.md) for detailed setup.

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
GPS Module
├─ Requires: CAN bus
├─ Optional: UART for serial input
└─ Outputs: Position data to CAN

Temperature Sensor
├─ Requires: CAN bus, I2C or SPI sensor
└─ Outputs: Temperature readings to CAN

Power Control Module (Central Hub)
├─ Requires: CAN bus, relay outputs
├─ Depends on: Multiple sensors for logic
└─ Outputs: Power state commands to relays

Heater Control
├─ Requires: CAN bus, PWM output
├─ Depends on: Temperature sensor feedback
└─ Outputs: Heating element control

User Interface
├─ Requires: CAN bus, buttons/display
├─ Depends on: Status from other modules
└─ Outputs: User commands to CAN
```

## Hardware Specifications

### Common Hardware
- **Microcontroller**: ESP32 or ESP32-S3
- **Operating Voltage**: 3.3V (internal), 5-24V (input with regulator)
- **Power Consumption**: 80mA typical, 10µA deep sleep
- **Storage**: 4MB Flash (variable)
- **RAM**: 320KB (variable)
- **GPIO**: 25+ (variable per module)
- **SPI**: 2-3 buses
- **I2C**: 2 buses
- **UART**: 2-3 buses
- **PWM**: 16 channels
- **ADC**: 12-bit, 8 channels

### CAN Interface
- **Transceiver**: MCP2515 (SPI) or TJA1050 (built-in)
- **Speed**: Configurable (500 kbps or 1 Mbps typical)
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

- `TrailCurrentGpsModule/`
- `TrailCurrentTempSensor/`
- `TrailCurrentAirQualityModule/`
- `TrailCurrentCabinetAndDoorSensor/`
- `TrailCurrentShuntGateway/`
- `TrailCurrentPowerControlModule/`
- `TrailCurrentElectricHeaterControl/`
- `TrailCurrentVehicleLeveler/`
- `TrailCurrentMpptCanGateway/`
- `TrailCurrentBtGateway/`
- `TrailCurrentCanEspNowGateway/`
- `TrailCurrentSevenPinTrailerMonitor/`
- `TrailCurrentEightButtonPanel/`
- `TrailCurrentWallMountedDisplay/`
- `TrailCurrentWallMountedDisplaySunton7Inch/`
- `TrailCurrentEspNowRemoteControl/`
- `TrailCurrentWaveshareEsp32s3Remote/`

---

## See Also

- [MODULE_CATEGORIES.md](MODULE_CATEGORIES.md) - Organized by function
- [Firmware/ESP_IDF_Setup.md](Firmware/ESP_IDF_Setup.md) - Setting up development
- [07_Development/DEVELOPMENT_SETUP.md](../07_Development/DEVELOPMENT_SETUP.md) - Full dev environment
- [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) - CAN message definitions
- [10_Reference/GPIO_PIN_MAPPING.md](../10_Reference/GPIO_PIN_MAPPING.md) - Pin configurations
