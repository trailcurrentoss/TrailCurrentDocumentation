# Hardware Module Categories

Organizational guide to TrailCurrent hardware modules by function and purpose.

## Sensor Modules (Data Collection)

These modules collect data from the environment and vehicle systems.

### GPS Module
- **Purpose**: Vehicle location tracking
- **Inputs**: GPS antenna, power
- **Outputs**: CAN messages with latitude, longitude, altitude, satellites
- **CAN ID Range**: 0x100-0x10F
- **Documentation**: [GPS_Module.md](GPS_Module.md)
- **Source**: `/Product/TrailCurrentGpsModule/`

### Temperature Sensor
- **Purpose**: Environmental and system temperature monitoring
- **Inputs**: One or more temperature sensors (DS18B20, DHT22, etc.)
- **Outputs**: CAN messages with temperature data
- **CAN ID Range**: 0x120-0x12F
- **Documentation**: [Temperature_Sensor.md](Temperature_Sensor.md)
- **Source**: `/Product/TrailCurrentTempSensor/`

### Air Quality Module
- **Purpose**: Monitor air quality, pollution, and particulates
- **Inputs**: PM2.5/PM10 sensor, optional gas sensors
- **Outputs**: CAN messages with AQI and particulate readings
- **CAN ID Range**: 0x130-0x13F
- **Documentation**: [Air_Quality_Module.md](Air_Quality_Module.md)
- **Source**: `/Product/TrailCurrentAirQualityModule/`

### Cabinet & Door Sensor
- **Purpose**: Detect when cabinet doors or enclosures are opened
- **Inputs**: Magnetic switches or motion sensors
- **Outputs**: CAN messages with door/cabinet state
- **CAN ID Range**: 0x140-0x14F
- **Documentation**: [Cabinet_Door_Sensor.md](Cabinet_Door_Sensor.md)
- **Source**: `/Product/TrailCurrentCabinetAndDoorSensor/`

### Shunt Gateway
- **Purpose**: Monitor power consumption and battery voltage
- **Inputs**: Current shunt for measuring current draw
- **Outputs**: CAN messages with voltage, current, power
- **CAN ID Range**: 0x150-0x15F
- **Documentation**: [Shunt_Gateway.md](Shunt_Gateway.md)
- **Source**: `/Product/TrailCurrentShuntGateway/`

---

## Control Modules (Action Execution)

These modules execute commands and control physical systems.

### Power Control Module (PCM)
- **Purpose**: Main power distribution and management
- **Inputs**: CAN commands from user interface or cloud
- **Outputs**: Relay activations, status on CAN
- **Power**: Typically 12V or 24V vehicle power
- **CAN ID Range**: 0x200-0x20F (commands), 0x280-0x28F (status)
- **Documentation**: [Power_Control_Module.md](Power_Control_Module.md)
- **Source**: `/Product/TrailCurrentPowerControlModule/`
- **Key Feature**: Central hub for power distribution to other devices

### Electric Heater Control
- **Purpose**: Control heating elements (e.g., water heater, space heater)
- **Inputs**: CAN commands, temperature sensor feedback
- **Outputs**: PWM output to heating element, status on CAN
- **CAN ID Range**: 0x210-0x21F (commands), 0x290-0x29F (status)
- **Documentation**: [Electric_Heater_Control.md](Electric_Heater_Control.md)
- **Source**: `/Product/TrailCurrentElectricHeaterControl/`
- **Key Feature**: Temperature control with safety limits

### Vehicle Leveler
- **Purpose**: Automatic leveling of vehicle (e.g., jacks)
- **Inputs**: CAN commands, optional inclinometer feedback
- **Outputs**: PWM output to leveling motors, status on CAN
- **CAN ID Range**: 0x220-0x22F (commands), 0x2A0-0x2AF (status)
- **Documentation**: [Vehicle_Leveler.md](Vehicle_Leveler.md)
- **Source**: `/Product/TrailCurrentVehicleLeveler/`
- **Key Feature**: Auto-leveling with mechanical fail-safes

### MPPT CAN Gateway
- **Purpose**: Interface with MPPT solar charge controller
- **Inputs**: CAN bus communication with MPPT device
- **Outputs**: Solar charging data, control commands to MPPT
- **CAN ID Range**: 0x230-0x23F
- **Documentation**: [MPPT_CAN_Gateway.md](MPPT_CAN_Gateway.md)
- **Source**: `/Product/TrailCurrentMpptCanGateway/`
- **Key Feature**: Bridges MPPT controller to TrailCurrent system

---

## Communication/Gateway Modules (Integration)

These modules provide connectivity and bridge external devices.

### BT Gateway
- **Purpose**: Enable Bluetooth communication with mobile devices
- **Inputs**: Wireless Bluetooth signal
- **Outputs**: CAN bus messages, status/telemetry
- **CAN ID Range**: 0x500-0x50F
- **Documentation**: [BT_Gateway.md](BT_Gateway.md)
- **Source**: `/Product/TrailCurrentBtGateway/`
- **Key Feature**: Direct mobile app communication without internet

### CAN/EspNow Gateway
- **Purpose**: Extend CAN bus range and provide redundant communication
- **Inputs**: CAN bus, WiFi/EspNow signals
- **Outputs**: Relayed CAN messages, status on CAN
- **CAN ID Range**: 0x510-0x51F
- **Documentation**: [CAN_EspNow_Gateway.md](CAN_EspNow_Gateway.md)
- **Source**: `/Product/TrailCurrentCanEspNowGateway/`
- **Key Feature**: Mesh networking with ESP-NOW protocol

### External Systems Monitor (7-pin Connector Module)
- **Purpose**: Monitor external systems via standard connectors (7-pin and others)
- **Inputs**: 7-pin connector, CAN bus
- **Outputs**: External system status on CAN (lights, brakes, backup systems, etc.)
- **CAN ID Range**: 0x520-0x52F
- **Documentation**: [Trailer_Monitoring.md](Trailer_Monitoring.md)
- **Source**: `/Product/TrailCurrentSevenPinTrailerMonitor/`
- **Key Feature**: Standard connector integration for external subsystems

---

## User Interface Modules (Interaction)

These modules allow users to view status and issue commands.

### Eight Button Panel
- **Purpose**: Physical button interface for basic control
- **Inputs**: 8 physical buttons
- **Outputs**: CAN commands based on button presses
- **CAN ID Range**: 0x400-0x40F
- **Documentation**: [Eight_Button_Panel.md](Eight_Button_Panel.md)
- **Source**: `/Product/TrailCurrentEightButtonPanel/`
- **Key Feature**: Simple, reliable physical control

### Wall-Mounted Display (Standard)
- **Purpose**: Display vehicle status and provide control interface
- **Inputs**: CAN bus for status data
- **Outputs**: CAN commands from user touch/buttons
- **Display**: LCD or LED screen with buttons
- **CAN ID Range**: 0x410-0x41F
- **Documentation**: [Wall_Mounted_Display.md](Wall_Mounted_Display.md)
- **Source**: `/Product/TrailCurrentWallMountedDisplay/`
- **Key Feature**: Compact dashboard for basic control

### Wall-Mounted Display (7" Sunton)
- **Purpose**: High-resolution touchscreen display
- **Inputs**: CAN bus, WiFi (optional)
- **Outputs**: CAN commands from touchscreen
- **Display**: 7" high-resolution touchscreen
- **CAN ID Range**: 0x420-0x42F
- **Documentation**: [Wall_Mounted_Display_Sunton.md](Wall_Mounted_Display_Sunton.md)
- **Source**: `/Product/TrailCurrentWallMountedDisplaySunton7Inch/`
- **Key Feature**: Full web-based dashboard interface

### EspNow Remote Control
- **Purpose**: Wireless remote control using ESP-NOW protocol
- **Inputs**: Physical buttons
- **Outputs**: EspNow commands, relayed to CAN via gateway
- **Range**: 100+ meters (line of sight)
- **CAN ID Range**: 0x430-0x43F
- **Documentation**: [EspNow_Remote_Control.md](EspNow_Remote_Control.md)
- **Source**: `/Product/TrailCurrentEspNowRemoteControl/`
- **Key Feature**: Mesh-capable wireless remote

### Waveshare ESP32S3 Remote
- **Purpose**: Feature-rich wireless remote with specific hardware
- **Inputs**: Buttons, optional display
- **Outputs**: Wireless commands, status feedback
- **CAN ID Range**: 0x440-0x44F
- **Documentation**: [Waveshare_ESP32S3_Remote.md](Waveshare_ESP32S3_Remote.md)
- **Source**: `/Product/TrailCurrentWaveshareEsp32s3Remote/`
- **Key Feature**: Advanced hardware platform for custom remotes

---

## Functional Use Cases

### Basic Vehicle Monitoring
Minimum modules needed:
- Power Control Module (PCM)
- GPS Module
- Temperature Sensor
- 8-Button Panel or wall display

### Full Environmental Control
Add to above:
- Electric Heater Control
- Shunt Gateway
- Air Quality Module
- 7" Wall Display

### Remote-Capable System
Add to above:
- CAN/EspNow Gateway
- BT Gateway
- Wireless remote

### External Systems Integration
Add support for external systems/subsystems:
- 7-Pin Connector Monitor
- External systems monitoring dashboard

---

## Module Dependencies & Interactions

```
┌─────────────────────────────────────────┐
│  Power Control Module (PCM)             │
│  Central Hub                            │
└────────────┬────────────────────────────┘
             │
     ┌───────┼──────────┐
     │       │          │
     ▼       ▼          ▼
┌────────┐ ┌───────┐ ┌──────────────┐
│Heater  │ │Leveler│ │External      │
│Control │ │Control│ │Systems       │
└────────┘ └───────┘ └──────────────┘
     ▲       ▲          ▲
     │       │          │
     └───────┼──────────┘
             │
     Feedback Sensors:
     ├─ Temperature
     ├─ Shunt (Power)
     ├─ GPS
     ├─ Air Quality
     └─ Door/Cabinet

User Interfaces:
├─ 8-Button Panel
├─ Wall Display
├─ 7" Touch Display
├─ EspNow Remote
└─ Waveshare Remote
     │
     └─ All can control via PCM
```

---

## Commonalities Across Categories

### All Modules Share
1. **CAN Bus Communication** - Standard protocol
2. **Configuration in NVS** - Store settings
3. **Status LED** - RGB LED for diagnostics
4. **OTA Update Support** - Firmware updates
5. **Deep Sleep Mode** - Power saving
6. **Debug Logging** - Serial or CAN-based

### Hardware Commonalities
1. **ESP32 or ESP32-S3** - Same MCU platform
2. **3.3V Digital Logic** - Compatible with all
3. **CAN Transceiver** - MCP2515 or TJA1050
4. **Voltage Regulator** - 12/24V to 3.3V
5. **Power Switch** - For deep sleep control

---

See Also:
- [README.md](README.md) - Module overview
- [Firmware/ESP_IDF_Setup.md](Firmware/ESP_IDF_Setup.md) - Development setup
- [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) - CAN message definitions
