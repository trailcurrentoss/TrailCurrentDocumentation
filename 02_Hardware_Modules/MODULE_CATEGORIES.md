# Hardware Module Categories

Organizational guide to TrailCurrent hardware modules by function and purpose.

## Sensor Modules (Data Collection)

These modules collect data from the environment and vehicle systems.

### TrailCurrent Bearing (GNSS Module)
- **Purpose**: GPS location, heading, altitude, and precise timing
- **Inputs**: GNSS antenna, power
- **Outputs**: CAN messages with latitude, longitude, altitude, heading, satellites, timing
- **CAN ID Range**: 0x100-0x10F
- **Documentation**: [Bearing.md](Bearing.md)
- **Source**: `/Product/TrailCurrentGnssModule/`

### TrailCurrent Borealis (Air Quality & Environment)
- **Purpose**: Monitor temperature, humidity, CO2, and indoor air quality
- **Inputs**: Integrated environmental sensors (temp, humidity, CO2)
- **Outputs**: CAN messages with environmental readings
- **CAN ID Range**: 0x130-0x13F
- **Documentation**: [Borealis.md](Borealis.md)
- **Source**: `/Product/TrailCurrentBorealis/`

### TrailCurrent Picket (Cabinet & Door Sensors)
- **Purpose**: Monitor open/closed status of doors, windows, cabinets, and bays
- **Inputs**: Magnetic switches or motion sensors
- **Outputs**: CAN messages with door/cabinet state
- **CAN ID Range**: 0x140-0x14F
- **Documentation**: [Picket.md](Picket.md)
- **Source**: `/Product/TrailCurrentPicket/`

### TrailCurrent Ampline (Shunt Interface)
- **Purpose**: Track power consumption and state-of-charge via Victron Shunt
- **Inputs**: Victron Shunt connection
- **Outputs**: CAN messages with voltage, current, power, state-of-charge
- **CAN ID Range**: 0x150-0x15F
- **Documentation**: [Ampline.md](Ampline.md)
- **Source**: `/Product/TrailCurrentAmpline/`

### TrailCurrent Plateau (Vehicle Level Sensor)
- **Purpose**: Tilt/level measurement on both axes
- **Inputs**: Integrated inclinometer/accelerometer
- **Outputs**: CAN messages with level/tilt data
- **CAN ID Range**: 0x220-0x22F (commands), 0x2A0-0x2AF (status)
- **Documentation**: [Plateau.md](Plateau.md)
- **Source**: `/Product/TrailCurrentPlateau/`

---

## Control Modules (Action Execution)

These modules execute commands and control physical systems.

### TrailCurrent Torrent (Power Delivery Module)
- **Purpose**: 8-channel smart power distribution with on/off switching and PWM dimming
- **Inputs**: CAN commands from user interface or cloud
- **Outputs**: 8 switched/dimmed power channels, status on CAN
- **Power**: Typically 12V or 24V vehicle power
- **CAN ID Range**: 0x200-0x20F (commands), 0x280-0x28F (status)
- **Documentation**: [Torrent.md](Torrent.md)
- **Source**: `/Product/TrailCurrentTorrent/`
- **Key Feature**: Central hub for power distribution to other devices

### TrailCurrent Therma (Climate Relay Controller)
- **Purpose**: Dual-relay automatic heating/cooling to maintain set temperature
- **Inputs**: CAN commands, Borealis temperature feedback
- **Outputs**: Dual relay control for heating and cooling, status on CAN
- **CAN ID Range**: 0x210-0x21F (commands), 0x290-0x29F (status)
- **Documentation**: [Therma.md](Therma.md)
- **Source**: `/Product/TrailCurrentTherma/`
- **Key Feature**: Automatic temperature control with safety limits

### TrailCurrent Solstice (MPPT Solar Controller Interface)
- **Purpose**: Connect Victron MPPT solar charge controller and stream solar harvest data
- **Inputs**: Victron MPPT connection
- **Outputs**: Solar charging data on CAN
- **CAN ID Range**: 0x230-0x23F
- **Documentation**: [Solstice.md](Solstice.md)
- **Source**: `/Product/TrailCurrentSolstice/`
- **Key Feature**: Bridges Victron MPPT controller to TrailCurrent system

---

## Communication/Gateway Modules (Integration)

These modules provide connectivity and bridge external devices.

### TrailCurrent Aftline (Trailer Wiring Harness Monitor)
- **Purpose**: Monitor all 7 pins of trailer wiring (signals, lights, connection status)
- **Inputs**: 7-pin trailer connector, CAN bus
- **Outputs**: Trailer wiring status on CAN (lights, brakes, signals, connection)
- **CAN ID Range**: 0x520-0x52F
- **Documentation**: [Aftline.md](Aftline.md)
- **Source**: `/Product/TrailCurrentAftline/`
- **Key Feature**: Complete trailer wiring harness monitoring

### RV-C Gateway *(Coming Soon)*
- **Purpose**: RV-C protocol gateway for industry-standard RV device integration
- **Inputs**: RV-C bus, CAN bus
- **Outputs**: Bridged RV-C data on TrailCurrent CAN
- **CAN ID Range**: TBD
- **Documentation**: TBD
- **Source**: TBD
- **Key Feature**: Industry-standard RV device interoperability

---

## User Interface Modules (Interaction)

These modules allow users to view status and issue commands.

### TrailCurrent Tapper (8-Button Panel)
- **Purpose**: Physical button interface for Torrent commands
- **Inputs**: 8 physical buttons
- **Outputs**: CAN commands based on button presses
- **CAN ID Range**: 0x400-0x40F
- **Documentation**: [Tapper.md](Tapper.md)
- **Source**: `/Product/TrailCurrentTapper/`
- **Key Feature**: Simple, reliable physical control

### TrailCurrent Fireside (Wireless Touchscreen Display)
- **Purpose**: Wireless battery-powered touchscreen display with wall cradle
- **Inputs**: CAN bus for status data, WiFi
- **Outputs**: CAN commands from touchscreen
- **Display**: Touchscreen with wall cradle for charging
- **CAN ID Range**: 0x410-0x41F
- **Documentation**: [Fireside.md](Fireside.md)
- **Source**: `/Product/TrailCurrentFireside/`
- **Key Feature**: Portable wireless display that doubles as a remote

### TrailCurrent Milepost (Hardwired CAN Bus Touchscreen)
- **Purpose**: Always-on hardwired touchscreen display, mountable in multiple locations
- **Inputs**: CAN bus, hardwired power
- **Outputs**: CAN commands from touchscreen
- **Display**: 7" high-resolution touchscreen
- **CAN ID Range**: 0x420-0x42F
- **Documentation**: [Milepost.md](Milepost.md)
- **Source**: `/Product/TrailCurrentMilepost/`
- **Key Feature**: Full web-based dashboard interface, always powered

### TrailCurrent Spotter (In-Vehicle Trailer Monitor)
- **Purpose**: In-vehicle ESP32S3 display that monitors trailer status while towing
- **Inputs**: CAN bus, wireless connection to trailer modules
- **Outputs**: Real-time trailer status alerts
- **CAN ID Range**: 0x440-0x44F
- **Documentation**: [Spotter.md](Spotter.md)
- **Source**: `/Product/TrailCurrentSpotter/`
- **Key Feature**: Compact dash-mounted display for real-time trailer alerts

---

## Voice & AI Modules

### TrailCurrent Peregrine (AI Voice Assistant)
- **Purpose**: AI voice companion with system access and hands-free control
- **Inputs**: Microphone, system data via CAN/MQTT
- **Outputs**: Voice responses, system commands
- **Wake Word**: "Hey Peregrine"
- **Documentation**: [Peregrine.md](Peregrine.md)
- **Source**: `/Product/TrailCurrentPeregrine/`
- **Key Feature**: Hands-free intelligent control of entire TrailCurrent system

---

## Functional Use Cases

### Basic Vehicle Monitoring
Minimum modules needed:
- Torrent (power delivery)
- Bearing (GNSS)
- Borealis (environment)
- Tapper or Fireside (user interface)

### Full Environmental Control
Add to above:
- Therma (climate control)
- Ampline (power monitoring)
- Milepost (always-on display)

### Towing Setup
Add to above:
- Aftline (trailer wiring monitor)
- Spotter (in-vehicle trailer status display)

### Complete System
Add to above:
- Solstice (solar monitoring)
- Picket (cabinet & door sensors)
- Plateau (vehicle leveling)
- Peregrine (voice assistant)
- Fireside (portable wireless display)

---

## Module Dependencies & Interactions

```
┌─────────────────────────────────────────┐
│  Torrent (Power Delivery Module)        │
│  Central Hub - 8 switched channels      │
└────────────┬────────────────────────────┘
             │
     ┌───────┼──────────┐
     │       │          │
     ▼       ▼          ▼
┌────────┐ ┌────────┐ ┌──────────────┐
│Therma  │ │Solstice│ │Aftline       │
│Climate │ │Solar   │ │Trailer       │
└────────┘ └────────┘ └──────────────┘
     ▲       ▲          ▲
     │       │          │
     └───────┼──────────┘
             │
     Feedback Sensors:
     ├─ Borealis (environment)
     ├─ Ampline (power/SoC)
     ├─ Bearing (GNSS)
     ├─ Plateau (level)
     └─ Picket (doors/cabinets)

User Interfaces:
├─ Tapper (8-button panel)
├─ Fireside (wireless touchscreen)
├─ Milepost (hardwired touchscreen)
└─ Spotter (trailer monitor display)
     │
     └─ All can control via Torrent

Voice & AI:
└─ Peregrine (voice assistant)
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
