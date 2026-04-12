# Hardware Module Categories

Organizational guide to TrailCurrent hardware modules by function and purpose.

## Sensor Modules (Data Collection)

These modules collect data from the environment and vehicle systems.

### TrailCurrent Bearing (GNSS Module)
- **Purpose**: GPS location, heading, altitude, and precise timing
- **Hardware**: Waveshare ESP32-S3-RS485-CAN with DFRobot GNSS receiver
- **Framework**: ESP-IDF
- **Inputs**: GNSS antenna, power
- **Outputs**: CAN messages with date/time, satellites+speed+course, altitude, latitude+longitude
- **CAN IDs**: 0x06-0x09 (GpsDateTime, GpsSatSpeedCourseMode, GpsAltitude, GpsLatitudeLongitude)
- **Documentation**: [Bearing.md](Bearing.md)
- **Source**: `/Product/TrailCurrentBearing/`

### TrailCurrent Borealis (Air Quality & Environment)
- **Purpose**: Monitor temperature, humidity, TVOC, and eCO2
- **Hardware**: ESP32-S3-Zero
- **Framework**: ESP-IDF
- **Inputs**: SHT31-D (temperature/humidity) + SGP30 (TVOC/eCO2) over I²C
- **Outputs**: CAN messages with environmental readings
- **CAN IDs**: 0x1F (EnvironmentSensorData); accepts 0x21 (BorealisCalibration)
- **Documentation**: [Borealis.md](Borealis.md)
- **Source**: `/Product/TrailCurrentBorealis/`

### TrailCurrent Picket (Cabinet & Door Sensors)
- **Purpose**: Monitor open/closed status of doors, windows, cabinets, and bays
- **Hardware**: Waveshare ESP32-S3-RS485-CAN (off-the-shelf board with onboard CAN transceiver)
- **Framework**: ESP-IDF
- **Inputs**: Up to 13 magnetic reed switches (via pin header, internal pull-ups, no external resistors)
- **Outputs**: CAN messages with door/cabinet state
- **Addressing**: Compile-time `PICKET_ADDRESS` build flag (0-7, up to 8 modules per bus)
- **CAN ID Range**: 0x0A-0x11
- **Documentation**: [Picket.md](Picket.md)
- **Source**: `/Product/TrailCurrentPicket/`

### TrailCurrent Ampline (Shunt Interface)
- **Purpose**: Track power consumption and state-of-charge via Victron BMV SmartShunt
- **Hardware**: Waveshare ESP32-S3-RS485-CAN
- **Framework**: ESP-IDF
- **Inputs**: Victron BMV SmartShunt via VE.Direct (19200 baud)
- **Outputs**: CAN messages with voltage, current, power, state-of-charge, consumed Ah
- **CAN IDs**: 0x23-0x24 (BatteryShuntData1, BatteryShuntData2)
- **Documentation**: [Ampline.md](Ampline.md)
- **Source**: `/Product/TrailCurrentAmpline/`

### TrailCurrent Plateau (Vehicle Level Sensor)
- **Purpose**: Tilt/level measurement on both axes with per-corner height calculation
- **Hardware**: ESP32-S3-Zero with Adafruit BNO055 IMU
- **Framework**: ESP-IDF
- **Inputs**: BNO055 orientation data; CAN leveling configuration (0x20)
- **Outputs**: CAN messages with tilt, per-corner heights, and status
- **CAN IDs**: 0x30 (TiltData), 0x31 (CornerData), 0x32 (StatusData); accepts 0x20 (LevelingConfig)
- **Documentation**: [Plateau.md](Plateau.md)
- **Source**: `/Product/TrailCurrentPlateau/`

### TrailCurrent Reservoir (Water Tank Level Monitor)
- **Purpose**: Report fill level for up to 3 water tanks (fresh, grey, black) via contactless sensors
- **Hardware**: Waveshare ESP32-S3-RS485-CAN
- **Framework**: ESP-IDF
- **Inputs**: Up to 4 contactless level sensors per tank (25%, 50%, 75%, 100%)
- **Outputs**: CAN messages with per-tank fill percentages
- **CAN IDs**: 0x3E (WaterTankLevels)
- **Documentation**: [Reservoir.md](Reservoir.md)
- **Source**: `/Product/TrailCurrentReservoir/`
- **Key Feature**: Percentage-based protocol allows future migration to analog sensors with no CAN changes

---

## Control Modules (Action Execution)

These modules execute commands and control physical systems.

### TrailCurrent Torrent (Power Delivery Module)
- **Purpose**: 8-channel smart power distribution with on/off switching and PWM dimming
- **Hardware**: ESP32 (WROOM)
- **Framework**: ESP-IDF
- **Inputs**: CAN commands from user interface, vehicle compute, or cloud
- **Outputs**: 8 switched/dimmed power channels, status on CAN
- **Power**: Typically 12V or 24V vehicle power
- **Addressing**: Compile-time address (0-2) — up to 3 modules per bus via `build-all.sh`
- **CAN IDs**: 0x15-0x17 (brightness), 0x18-0x1A (toggle), 0x1B-0x1D (status), 0x33-0x35 (light sequence)
- **Documentation**: [Torrent.md](Torrent.md)
- **Source**: `/Product/TrailCurrentTorrent/`
- **Key Feature**: Central hub for power distribution; animated light sequences supported

### TrailCurrent Therma (Closed-Loop Thermostat)
- **Purpose**: Three-board closed-loop thermostat. Controller owns the authoritative desired temperature, mode, and threshold; drives separate heater and cooler relay boards over GPIO. Heating and cooling are mutually exclusive and enforced by the controller. Hysteresis prevents chatter
- **Hardware**: Waveshare ESP32-S3-RS485-CAN (controller, on CAN bus) + 2× Waveshare ESP32-S3-Relay-1CH (heater + cooler, off CAN bus)
- **Inputs**: Borealis EnvironmentSensorData (`0x1F`) for current temperature; ThermaSetDesiredRequest (`0x41`) and ThermaSetThresholdRequest (`0x42`) from any device on the bus
- **Outputs**: ThermaDesiredTemperature (`0x3F`) and ThermaStatus (`0x40`) at 1 Hz, broadcast by the controller as authoritative truth; GPIO drive lines to the two relay boards
- **CAN IDs**: `0x3F`-`0x42`
- **Documentation**: [Therma.md](Therma.md)
- **Source**: `/Product/TrailCurrentTherma/`
- **Key Feature**: Authoritative state owner pattern (same as Torrent / Switchback) — every other device displays the value Therma broadcasts and sends change requests rather than holding a local copy

### TrailCurrent Solstice (MPPT Solar Controller Interface)
- **Purpose**: Connect Victron MPPT solar charge controller and SmartShunt; stream solar and battery data
- **Hardware**: Waveshare ESP32-S3-RS485-CAN
- **Framework**: ESP-IDF
- **Inputs**: Victron MPPT via VE.Direct (TEXT + HEX); optional SmartShunt polling
- **Outputs**: Solar charging data and external shunt data on CAN
- **CAN IDs**: 0x2B (ShuntExtLive), 0x2C-0x2D (SolarMpptData1/2), 0x2F (ShuntExtHistory); accepts 0x2E (SolarLoadControl)
- **Documentation**: [Solstice.md](Solstice.md)
- **Source**: `/Product/TrailCurrentSolstice/`
- **Key Feature**: Bridges Victron MPPT and SmartShunt to TrailCurrent; supports load-output control

### TrailCurrent Switchback (Relay Module)
- **Purpose**: High-current relay switching for loads that don't fit the Torrent PWM profile
- **Hardware**: Waveshare ESP32-S3-ETH-8DI-8RO-C (8 dry-contact relay outputs)
- **Framework**: ESP-IDF
- **Inputs**: CAN toggle commands from UI/Headwaters
- **Outputs**: Relay state bitmask on CAN
- **Addressing**: Compile-time address (0-2) — up to 3 modules per bus via `build-all.sh`
- **CAN IDs**: 0x25-0x27 (toggle commands), 0x28-0x2A (status reports)
- **Documentation**: [Switchback.md](Switchback.md)
- **Source**: `/Product/TrailCurrentSwitchback/`

---

## Communication/Gateway Modules (Integration)

These modules provide connectivity and bridge external devices.

### TrailCurrent Aftline (Trailer Wiring Harness Monitor)
- **Purpose**: Monitor all 7 pins of trailer wiring (signals, lights, connection status)
- **Hardware**: Waveshare ESP32-S3-RS485-CAN
- **Framework**: ESP-IDF
- **Inputs**: ADC voltage sensing plus digital turn/brake/light lines from a 7-pin trailer connector
- **Outputs**: Trailer wiring status on CAN (lights, brakes, signals, connection)
- **CAN IDs**: 0x10 (TrailerStatus)
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
- **Purpose**: Physical 8-button panel that emits Torrent or Switchback commands
- **Hardware**: ESP32 (WROOM)
- **Framework**: ESP-IDF
- **Inputs**: 8 physical buttons
- **Outputs**: Brightness/toggle commands on CAN (target-specific)
- **Addressing**: Target (Torrent or Switchback) × instance address (0-2) selected at build time — 6 binaries from `build-all.sh`
- **CAN IDs**: Depends on target/address (0x15-0x1A for Torrent targets, 0x25-0x27 for Switchback targets)
- **Documentation**: [Tapper.md](Tapper.md)
- **Source**: `/Product/TrailCurrentTapper/`
- **Key Feature**: Simple, reliable physical control with pick-your-target build system

### TrailCurrent Fireside (Wireless Touchscreen Display)
- **Purpose**: 7" touchscreen status + control display that runs wirelessly on battery or plugged in
- **Hardware**: Waveshare ESP32-P4-WiFi6-Touch-LCD-7B (ESP32-P4 + ESP-Hosted SDIO WiFi co-processor)
- **Framework**: ESP-IDF (LVGL UI generated via EEZ Studio)
- **Inputs**: MQTT from Headwaters over WiFi/TLS; SD card `config.env` for provisioning
- **Outputs**: MQTT commands to Headwaters (which bridges to CAN)
- **Display**: 7" capacitive touch LCD with wall cradle for charging
- **Documentation**: [Fireside.md](Fireside.md)
- **Source**: `/Product/TrailCurrentFireside/`
- **Key Feature**: Portable wireless display that doubles as a room controller

### TrailCurrent Milepost (Hardwired Wall Display)
- **Purpose**: Always-on hardwired touchscreen display, mountable in multiple locations
- **Hardware**: Waveshare ESP32-S3-Touch-LCD-7 (V1.2) — 7" capacitive touch LCD with CH422G IO expander
- **Framework**: ESP-IDF (LVGL UI generated via EEZ Studio)
- **Inputs**: CAN bus, hardwired 12V power
- **Outputs**: CAN commands from touchscreen
- **Documentation**: [Milepost.md](Milepost.md)
- **Source**: `/Product/TrailCurrentMilepost/`
- **Key Feature**: Full LVGL dashboard interface, always powered, brightness dimming

### TrailCurrent Spotter (In-Vehicle Trailer Monitor)
- **Purpose**: In-vehicle 4.3" display that monitors trailer status over MQTT while towing
- **Hardware**: ESP32-S3 based 4.3" LCD board
- **Framework**: PlatformIO wrapping ESP-IDF (LVGL v8 via EEZ Studio) — the only PlatformIO project in the platform
- **Inputs**: MQTT over WiFi
- **Outputs**: Real-time trailer status dashboard, light/relay remote
- **Documentation**: [Spotter.md](Spotter.md)
- **Source**: `/Product/TrailCurrentSpotter/`
- **Key Feature**: Compact dash-mounted display for real-time trailer alerts and remote control

---

## Voice & AI Modules

### TrailCurrent Peregrine (AI Voice Assistant)
- **Purpose**: Fully-local AI voice companion with system access and hands-free control
- **Hardware**: Radxa Dragon Q6A (Qualcomm QCS6490 SoC with Hexagon NPU, 8 GB)
- **Framework**: Custom Ubuntu Noble 24.04 image running a Python pipeline under systemd
- **Inputs**: Microphone, system data via MQTT from Headwaters
- **Outputs**: Voice responses, MQTT device commands
- **Stack**: openWakeWord → faster-whisper → Llama 3.2 1B → Piper TTS (all local, no cloud)
- **Wake Word**: Configurable (default "Hey Peregrine")
- **Documentation**: [Peregrine.md](Peregrine.md)
- **Source**: `/Product/TrailCurrentPeregrine/`
- **Key Feature**: Fully offline pipeline — never leaves the vehicle (Core Principle: Data Privacy First)

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
1. **ESP32 family** - ESP32 WROOM, ESP32-S3, or ESP32-P4 (one module) across the line
2. **3.3V Digital Logic** - Compatible across modules
3. **CAN Transceiver** - SN65HVD230 (external) or integrated on Waveshare RS485-CAN / Relay boards
4. **Voltage Regulator** - 12/24V to 3.3V (on-board for most Waveshare carrier boards)
5. **OTA-capable partition table** - Dual OTA + factory fallback on nearly every module

---

See Also:
- [README.md](README.md) - Module overview
- [Firmware/ESP_IDF_Setup.md](Firmware/ESP_IDF_Setup.md) - Development setup
- [10_Reference/CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md) - CAN message definitions
