# CAN Bus Reference

Complete reference for CAN message formats, IDs, and protocols used in TrailCurrent. This document is derived from the authoritative `TrailCurrent.dbc` file in the repository root.

## Overview

- **Protocol**: CAN 2.0A (standard frames)
- **Bitrate**: 500 kbps
- **Message Format**: Standard 11-bit identifiers
- **Termination**: 120Ω resistors at both ends of bus
- **Database Version**: 1.0.0

## Message Format

### CAN Frame Structure

```
[ID (11-bit)] [RTR] [IDE] [DLC] [Data (0-8 bytes)]
```

- **ID**: Message identifier (0x000-0x7FF)
- **RTR**: Remote Transmission Request bit
- **IDE**: Identifier Extension bit (0 for standard frames)
- **DLC**: Data Length Code (0-8 bytes)

## CAN ID Allocation

All TrailCurrent messages currently use IDs in the range 0x00-0x32 (decimal 0-50).

| CAN ID (hex) | CAN ID (dec) | Message Name | DLC | Sender | Cycle Time |
|--------------|-------------|--------------|-----|--------|------------|
| 0x00 | 0 | OtaUpdateNotification | 3 | Headwaters | Event-driven |
| 0x01 | 1 | WifiConfigProvisioning | 8 | Headwaters | Event-driven |
| 0x02 | 2 | DiscoveryTrigger | 0 | Headwaters | Event-driven |
| 0x03 | 3 | DiscoveryReset | 3 | Headwaters | Event-driven |
| 0x06 | 6 | GpsDateTime | 7 | Bearing | 1000 ms |
| 0x07 | 7 | GpsSatSpeedCourseMode | 6 | Bearing | 1000 ms |
| 0x08 | 8 | GpsAltitude | 4 | Bearing | 1000 ms |
| 0x09 | 9 | GpsLatitudeLongitude | 8 | Bearing | 1000 ms |
| 0x0A | 10 | PicketStatus0 | 2 | Picket | 200 ms |
| 0x0B | 11 | PicketStatus1 | 2 | Picket | 200 ms |
| 0x0C | 12 | PicketStatus2 | 2 | Picket | 200 ms |
| 0x0D | 13 | PicketStatus3 | 2 | Picket | 200 ms |
| 0x0E | 14 | PicketStatus4 | 2 | Picket | 200 ms |
| 0x0F | 15 | PicketStatus5 | 2 | Picket | 200 ms |
| 0x10 | 16 | PicketStatus6 | 2 | Picket | 200 ms |
| 0x11 | 17 | PicketStatus7 | 2 | Picket | 200 ms |
| 0x15 | 21 | BrightnessControl | 2 | Multiple senders | Event-driven |
| 0x18 | 24 | ToggleOnOffCommand | 2 | Multiple senders | Event-driven |
| 0x1B | 27 | DeviceStatusReport | 8 | Torrent | 33 ms |
| 0x1E | 30 | LightSequenceCommand | 1 | Multiple senders | Event-driven |
| 0x1F | 31 | EnvironmentSensorData | 8 | Borealis | 2000 ms |
| 0x20 | 32 | LevelingConfig | 7 | Headwaters | Event-driven |
| 0x23 | 35 | BatteryShuntData1 | 7 | Ampline | 33 ms |
| 0x24 | 36 | BatteryShuntData2 | 5 | Ampline | 33 ms |
| 0x25 | 37 | SwitchbackToggle0 | 2 | Multiple senders | Event-driven |
| 0x26 | 38 | SwitchbackToggle1 | 2 | Multiple senders | Event-driven |
| 0x27 | 39 | SwitchbackToggle2 | 2 | Multiple senders | Event-driven |
| 0x28 | 40 | SwitchbackStatus0 | 1 | Switchback | Event-driven |
| 0x29 | 41 | SwitchbackStatus1 | 1 | Switchback | Event-driven |
| 0x2A | 42 | SwitchbackStatus2 | 1 | Switchback | Event-driven |
| 0x2C | 44 | SolarMpptData1 | 7 | Solstice | 33 ms |
| 0x2D | 45 | SolarMpptData2 | 3 | Solstice | 33 ms |
| 0x30 | 48 | TiltData | 8 | Plateau | 500 ms |
| 0x31 | 49 | CornerData | 8 | Plateau | 500 ms |
| 0x32 | 50 | StatusData | 4 | Plateau | 2000 ms |

### Bus Nodes

| Node | Platform | Description |
|------|----------|-------------|
| Bearing | ESP32 | GPS receiver. Broadcasts GNSS position, altitude, speed, course, and date/time |
| Torrent | ESP32 | 8-channel PWM power distribution module |
| Tapper | ESP32 | Physical 8-button control panel |
| Ampline | ESP32 | Battery shunt monitor. Reads Victron BMV via VE.Direct serial (19200 baud) |
| Solstice | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) | Solar charge controller gateway. Reads Victron MPPT via VE.Direct serial (19200 baud). ESP-IDF firmware |
| Borealis | ESP32-S3-Zero | Environment/air quality sensor. DHT22 + SGP30 |
| Picket | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) | Cabinet/door sensor. Up to 13 reed switch inputs, NVS-addressed (8 modules max). ESP-IDF firmware |
| Switchback | ESP32-S3 | 6-channel relay module (Waveshare S3-Relay-6CH). Up to 3 on same bus |
| Plateau | ESP32-S3-Zero | Vehicle leveling with Adafruit BNO055 IMU |
| Aftline | ESP32-C6 | Trailer connector monitor. Under active development |
| Therma | ESP32-C6 | Heater control. Under active development |
| Headwaters | Raspberry Pi | CAN-to-MQTT bridge. Python script bridging CAN and MQTT (with TLS) |

---

## Module-Specific Messages

### OTA Update (0x00)

**OtaUpdateNotification** (3 bytes) — Sent by Headwaters when a new firmware build is available. Contains the last 3 bytes of the target device MAC address. Each device checks if the MAC matches its hostname (`esp32-XXYYZZ`). On match, the device connects to WiFi and checks for the OTA update.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | MacAddressByte1 | 7:0 | uint8 | 0-255 | First byte of target MAC address |
| 1 | MacAddressByte2 | 15:8 | uint8 | 0-255 | Second byte of target MAC address |
| 2 | MacAddressByte3 | 23:16 | uint8 | 0-255 | Third byte of target MAC address |

---

### Module Discovery (0x02, 0x03)

Self-discovery protocol for automatic module registration with Headwaters.

#### DiscoveryTrigger (0x02, 0 bytes)

Broadcast by Headwaters with no payload. All unconfigured modules respond by connecting to WiFi and advertising a `_trailcurrent._tcp` mDNS service with TXT records:

| TXT Key | Example Value | Description |
|---------|---------------|-------------|
| `type`  | `picket`      | Module type (compile-time) |
| `addr`  | `3`           | Instance address (compile-time) |
| `canid` | `0x0D`        | CAN message ID |
| `fw`    | `1.0.0`       | Firmware version |

Headwaters browses mDNS, reads the TXT records, then sends `GET http://<hostname>.local/discovery/confirm` to acknowledge. The module marks itself configured in NVS and tears down WiFi. Already-configured modules ignore this message.

#### DiscoveryReset (0x03, 3 bytes)

Targeted reset using the same MAC-matching format as OTA (0x00). Clears the configured flag on one module so it responds to the next DiscoveryTrigger.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | MacAddressByte1 | 7:0 | uint8 | 0-255 | First byte of target MAC address |
| 1 | MacAddressByte2 | 15:8 | uint8 | 0-255 | Second byte of target MAC address |
| 2 | MacAddressByte3 | 23:16 | uint8 | 0-255 | Third byte of target MAC address |

---

### Bearing - GNSS Module (0x06-0x09)

GPS receiver module that reads GNSS data and broadcasts position, altitude, speed, course, and date/time. Transmit-only device. All messages sent at 1 Hz (1000 ms cycle).

#### GpsDateTime (0x06, 7 bytes)

Date and time from the GNSS receiver in UTC.

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0-1 | Year | 7:0, 15:8 | uint16 BE | 2000-2099 | year | Calendar year (e.g., 2025) |
| 2 | Month | 23:16 | uint8 | 1-12 | month | Month of year |
| 3 | Day | 31:24 | uint8 | 1-31 | day | Day of month |
| 4 | Hour | 39:32 | uint8 | 0-23 | hour | Hour of day |
| 5 | Minute | 47:40 | uint8 | 0-59 | min | Minute of hour |
| 6 | Second | 55:48 | uint8 | 0-59 | sec | Second of minute |

#### GpsSatSpeedCourseMode (0x07, 6 bytes)

Navigation data: satellites, speed, heading, and constellation mode.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0 | NumSatellitesUsed | 7:0 | uint8 | 1 | 0-255 | satellites | Satellites used for position fix |
| 1-2 | SpeedOverGround | 15:8, 23:16 | uint16 BE | 0.01 | 0-655.35 | knots | Speed (raw value / 100 = knots) |
| 3-4 | CourseOverGround | 31:24, 39:32 | uint16 BE | 0.1 | 0-6553.5 | deg | Heading (raw / 10 = degrees, 0=N 90=E 180=S 270=W) |
| 5 | GnssMode | 47:40 | uint8 | 1 | 0-7 | - | Active GNSS constellation (see enum below) |

**GnssMode values:**

| Value | Constellation |
|-------|--------------|
| 0 | No constellation active |
| 1 | GPS |
| 2 | BeiDou |
| 3 | GPS + BeiDou |
| 4 | GLONASS |
| 5 | GPS + GLONASS |
| 6 | BeiDou + GLONASS |
| 7 | GPS + BeiDou + GLONASS |

#### GpsAltitude (0x08, 4 bytes)

Altitude above mean sea level.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0-3 | Altitude | 7:0 through 31:24 | uint32 BE | 0.01 | 0-42949672.95 | m | Altitude (raw / 100 = meters). E.g., 152340 = 1523.40 m |

#### GpsLatitudeLongitude (0x09, 8 bytes)

Position coordinates. Each coordinate is a sign byte followed by a 24-bit absolute value.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0 | LatitudeSign | 7:0 | uint8 | 1 | 0-1 | - | 0 = North (positive), 1 = South (negative) |
| 1-3 | LatitudeRaw | 15:8 through 31:24 | uint24 BE | 0.0001 | 0-1677.7215 | deg | Absolute latitude (raw / 10000 = degrees) |
| 4 | LongitudeSign | 39:32 | uint8 | 1 | 0-1 | - | 0 = East (positive), 1 = West (negative) |
| 5-7 | LongitudeRaw | 47:40 through 63:56 | uint24 BE | 0.0001 | 0-1677.7215 | deg | Absolute longitude (raw / 10000 = degrees) |

**Decoding example:** Sign=0, Raw=339876 means +33.9876 degrees North.

---

### Picket - Cabinet & Door Sensors (0x0A-0x11)

Monitors up to 13 magnetic reed switch inputs per module. Uses the Waveshare ESP32-S3-RS485-CAN board with onboard TJA1051 CAN transceiver. Module address (0-7) is set at compile time via `PICKET_ADDRESS` build flag (`idf.py build -DPICKET_ADDRESS=N`), allowing up to 8 Picket modules on the same bus. CAN IDs 0x0A through 0x11 share identical signal layout. Sent at 5 Hz (200 ms cycle). All reed switch inputs use internal pull-ups with no external resistors required.

#### PicketStatus (0x0A-0x11, 2 bytes each)

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | DoorStatus1to8 | 7:0 | uint8 bitmask | 0-255 | Bit 0 = door 1 (RSW01) ... bit 7 = door 8 (RSW08). 1 = open, 0 = closed |
| 1 | DoorStatus9to13 | 15:8 | uint8 bitmask | 0-31 | Bits 0-4 = doors 9-13 (RSW09-RSW13). Bits 5-7 reserved |

**Encoding:** 1 = door open (reed switch open, no magnet nearby). 0 = door closed (reed switch closed, magnet present).

**PICKET_ADDRESS to CAN ID mapping:**

| Address | CAN ID | Message Name |
|---------|--------|--------------|
| 0 (default) | 0x0A | PicketStatus0 |
| 1 | 0x0B | PicketStatus1 |
| 2 | 0x0C | PicketStatus2 |
| 3 | 0x0D | PicketStatus3 |
| 4 | 0x0E | PicketStatus4 |
| 5 | 0x0F | PicketStatus5 |
| 6 | 0x10 | PicketStatus6 |
| 7 | 0x11 | PicketStatus7 |

---

### Torrent - Power Delivery Module (0x15, 0x18, 0x1B, 0x1E)

8-channel PWM power distribution module. Controls up to 8 MOSFET outputs for lights and accessories. Receives commands from multiple senders including Tapper (physical buttons), Headwaters, Spotter, and other UI modules.

#### BrightnessControl (0x15, 2 bytes) — Event-driven

Sets PWM brightness for a specific output channel.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | ChannelIndex | 7:0 | uint8 | 0-7 | Output channel (0 = Output 1, 7 = Output 8) |
| 1 | BrightnessLevel | 15:8 | uint8 | 0-255 | PWM brightness (0 = off, 255 = full on) |

#### ToggleOnOffCommand (0x18, 2 bytes) — Event-driven

Toggles output channels on/off. Sent by multiple sources (Tapper, Headwaters, Spotter, etc.).

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | ButtonOrCommand | 7:0 | uint8 | 0-9 | Action (see values below) |
| 1 | CommandValue | 15:8 | uint8 | 0-255 | Parameter for special commands |

**ButtonOrCommand values:**

| Value | Action |
|-------|--------|
| 0-7 | Toggle corresponding output channel (0=Output1 ... 7=Output8) |
| 8 | All-channels command: CommandValue 0 = all off, 1 = all on |
| 9 | All channels on (when CommandValue=1) |

#### DeviceStatusReport (0x1B, 8 bytes) — 33 ms cycle (~30 Hz)

Current PWM state of all 8 outputs. Sent continuously.

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0 | Output1Value | 7:0 | uint8 | 0-255 | PWM | Output 1 brightness (0=off, 255=full) |
| 1 | Output2Value | 15:8 | uint8 | 0-255 | PWM | Output 2 brightness |
| 2 | Output3Value | 23:16 | uint8 | 0-255 | PWM | Output 3 brightness |
| 3 | Output4Value | 31:24 | uint8 | 0-255 | PWM | Output 4 brightness |
| 4 | Output5Value | 39:32 | uint8 | 0-255 | PWM | Output 5 brightness |
| 5 | Output6Value | 47:40 | uint8 | 0-255 | PWM | Output 6 brightness |
| 6 | Output7Value | 55:48 | uint8 | 0-255 | PWM | Output 7 brightness |
| 7 | Output8Value | 63:56 | uint8 | 0-255 | PWM | Output 8 brightness |

#### LightSequenceCommand (0x1E, 1 byte) — Event-driven

Triggers a pre-programmed light animation effect. Sequences run to completion before normal control resumes.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | SequenceType | 7:0 | uint8 | 0-1 | Animation type (see values below) |

**SequenceType values:**

| Value | Sequence | Description |
|-------|----------|-------------|
| 0 | Interior | Smooth fade in/out followed by chase pattern on outputs 5-8 |
| 1 | Exterior | Smooth fade in/out followed by alternating flash on outputs 3-4 |

---

### Borealis - Environment Sensor (0x1F)

Environment and air quality sensor module (ESP32-S3-Zero). Reads DHT22 for temperature/humidity and SGP30 for TVOC/eCO2. Bytes 0-3 are backwards-compatible with the predecessor AirQualityModule format. The SGP30 needs ~15 seconds warm-up; initial values are TVOC=0, eCO2=400.

#### EnvironmentSensorData (0x1F, 8 bytes) — 2000 ms cycle (0.5 Hz)

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0 | TemperatureCelsius | 7:0 | uint8 | 1 | -40 to 125 | degC | Whole degrees Celsius from DHT22 |
| 1 | TemperatureFahrenheit | 15:8 | uint8 | 1 | -40 to 257 | degF | Whole degrees Fahrenheit: F = (C * 9 + 3) / 5 + 32 |
| 2-3 | HumidityScaled | 23:16, 31:24 | uint16 BE | 0.01 | 0-100 | % | Relative humidity (raw / 100 = %). E.g., 5523 = 55.23% |
| 4-5 | TVOC | 39:32, 47:40 | uint16 BE | 1 | 0-60000 | ppb | Total VOCs from SGP30. 0-65 Excellent, 65-220 Good, 220-660 Moderate, 660-2200 Poor, 2200+ Unhealthy |
| 6-7 | eCO2 | 55:48, 63:56 | uint16 BE | 1 | 400-60000 | ppm | Equivalent CO2 from SGP30. <400 Low, 400-999 Normal, 1000-1999 High, >=2000 Alarm |

---

### Ampline - Battery Shunt Monitor (0x23-0x24)

Reads Victron BMV battery monitor data via VE.Direct serial protocol (UART at 19200 baud) and converts to CAN messages. Transmit-only on CAN. Both messages sent at ~30 Hz (33 ms cycle).

#### BatteryShuntData1 (0x23, 7 bytes)

Core battery metrics: voltage, current, and state of charge. Values use a whole+decimal split encoding.

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0 | BattVoltageWhole | 7:0 | uint8 | 0-255 | V | Battery voltage whole part |
| 1 | BattVoltageDecimal | 15:8 | uint8 | 0-99 | 0.01V | Battery voltage decimal (hundredths). Voltage = byte0 + byte1/100 |
| 2 | IsCurrentNegative | 23:16 | uint8 | 0-1 | - | 0 = charging (positive), 1 = discharging (negative) |
| 3 | ShuntCurrentWhole | 31:24 | uint8 | 0-255 | A | Current whole part |
| 4 | ShuntCurrentDecimal | 39:32 | uint8 | 0-99 | 0.01A | Current decimal (hundredths). Current = byte3 + byte4/100, apply sign |
| 5 | BattSocWhole | 47:40 | uint8 | 0-100 | % | State of charge whole part |
| 6 | BattSocDecimal | 55:48 | uint8 | 0-99 | 0.01% | SOC decimal. SOC = byte5 + byte6/100 |

**Example:** Bytes `13, 45, 1, 5, 30, 85, 50` = 13.45V, -5.30A discharge, 85.50% SOC.

#### BatteryShuntData2 (0x24, 5 bytes)

Power consumption and time remaining.

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0 | IsWattageNegative | 7:0 | uint8 | 0/255 | - | 0x00 = charging, 0xFF = discharging. Note: uses 0xFF, not 1 |
| 1-2 | Wattage | 15:8, 23:16 | uint16 BE | 0-65535 | W | Absolute power in watts |
| 3-4 | TimeToGo | 31:24, 39:32 | uint16 BE | 0-65535 | min | Minutes until battery depleted (65535 may = infinite/charging) |

**Example:** Bytes `0xFF, 0x01, 0x2C, 0x02, 0x58` = -300W discharge, 600 minutes remaining.

---

### Switchback - Relay Control Module (0x25-0x2A)

6-channel relay control module (ESP32-S3, Waveshare ESP32-S3-Relay-6CH). Controls 6 mechanical relays for switching high-current loads. Relays are binary on/off (not PWM). Supports up to 3 modules on the same bus, each with its own command/status CAN ID pair.

#### SwitchbackToggle (0x25-0x27, 2 bytes each) — Event-driven

Relay toggle commands. CAN IDs 0x25-0x27 share identical signal layout, each targeting a different physical module instance.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | RelayOrCommand | 7:0 | uint8 | 0-6 | Action (see values below) |
| 1 | CommandValue | 15:8 | uint8 | 0-1 | Parameter for all-channels command |

**RelayOrCommand values:**

| Value | Action |
|-------|--------|
| 0-5 | Toggle relay channel (0=CH1 ... 5=CH6) |
| 6 | All-channels command: CommandValue 0 = all off, 1 = all on |

**Module instance to CAN ID mapping:**

| Instance | Toggle Command ID | Status Report ID |
|----------|------------------|-----------------|
| 0 | 0x25 | 0x28 |
| 1 | 0x26 | 0x29 |
| 2 | 0x27 | 0x2A |

#### SwitchbackStatus (0x28-0x2A, 1 byte each) — Event-driven (on state change)

Relay status bitmask. Sent immediately when any relay state changes.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | RelayStateBitmask | 7:0 | uint8 bitmask | 0-63 | Bit 0=CH1 ... bit 5=CH6. 1=ON (energized), 0=OFF. Bits 6-7 reserved |

---

### Solstice - Solar MPPT Charge Controller (0x2C-0x2D)

Reads Victron MPPT solar charge controller data via VE.Direct serial protocol (UART at 19200 baud). Transmit-only on CAN. Both messages sent at ~30 Hz (33 ms cycle).

#### SolarMpptData1 (0x2C, 7 bytes)

Panel voltage, solar power, battery voltage, and charge status.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0 | PanelVoltageWhole | 7:0 | uint8 | 1 | 0-255 | V | Panel voltage whole part |
| 1 | PanelVoltageDecimal | 15:8 | uint8 | 1 | 0-99 | 0.01V | Panel voltage decimal. PanelV = byte0 + byte1/100 |
| 2-3 | SolarWattage | 23:16, 31:24 | uint16 BE | 1 | 0-65535 | W | Instantaneous solar power output |
| 4 | SolarBattVoltageWhole | 39:32 | uint8 | 1 | 0-255 | V | Battery voltage (charger side) whole part |
| 5 | SolarBattVoltageDecimal | 47:40 | uint8 | 1 | 0-99 | 0.01V | Battery voltage decimal |
| 6 | SolarChargeStatus | 55:48 | uint8 | 1 | 0-5 | - | Victron charge status enum (see below) |

**SolarChargeStatus values (Victron CS field):**

| Value | Status | Description |
|-------|--------|-------------|
| 0 | Off | Charger disabled or no sunlight |
| 2 | Fault | Error condition |
| 3 | Bulk | High current charging to quickly fill battery |
| 4 | Absorption | Voltage held constant, current decreasing |
| 5 | Float | Battery full, maintaining charge |

#### SolarMpptData2 (0x2D, 3 bytes)

Panel current.

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0 | IsPanelCurrentNegative | 7:0 | uint8 | 0-1 | - | 0 = positive (normal solar), 1 = negative (reverse) |
| 1 | PanelCurrentWhole | 15:8 | uint8 | 0-255 | A | Current whole part |
| 2 | PanelCurrentDecimal | 23:16 | uint8 | 0-99 | 0.01A | Current decimal. Current = byte1 + byte2/100, apply sign |

---

### Plateau - Vehicle Leveler (0x20, 0x30-0x32)

Vehicle leveling module (ESP32-S3-Zero with Adafruit BNO055 9-DOF IMU). Reads pitch and roll, computes per-corner height adjustments based on configurable vehicle dimensions. Supports three mounting orientations with automatic BNO055 axis remapping. Calibration offsets auto-saved to NVS when fully calibrated.

#### LevelingConfig (0x20, 7 bytes) — Event-driven command

Configuration command sent to Plateau.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | ConfigCommand | 7:0 | uint8 | 0-3 | Command type (see values below) |
| 1 | MountingSurface | 15:8 | uint8 | 0-2 | Mounting orientation (see values below) |
| 2-3 | VehicleLengthCm | 23:16, 31:24 | uint16 BE | 0-65535 cm | Front-to-rear axle length. Default 500 (5 m) |
| 4-5 | VehicleWidthCm | 39:32, 47:40 | uint16 BE | 0-65535 cm | Left-to-right wheel width. Default 200 (2 m) |
| 6 | SaveToNvs | 55:48 | uint8 | 0-1 | 0x01 = persist to NVS, 0x00 = session-only |

**ConfigCommand values:**

| Value | Command | Description |
|-------|---------|-------------|
| 1 | Set Vehicle Config | Apply bytes 1-6 (mounting, dimensions, save flag) |
| 2 | Request Status | Request immediate status report on CAN ID 0x32 |
| 3 | Zero/Tare | Calibration reset (reserved for future use) |

**MountingSurface values:**

| Value | Orientation | Description |
|-------|-------------|-------------|
| 0 | Floor | Horizontal mounting (default) |
| 1 | Left Wall | Vertical, left side |
| 2 | Right Wall | Vertical, right side |

#### TiltData (0x30, 8 bytes) — 500 ms cycle (2 Hz)

Vehicle pitch, roll, and computed height differences.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0-1 | PitchScaled | 7:0, 15:8 | int16 BE | 0.01 | -327.68 to 327.67 | deg | Pitch (raw / 100). Positive = front up |
| 2-3 | RollScaled | 23:16, 31:24 | int16 BE | 0.01 | -327.68 to 327.67 | deg | Roll (raw / 100). Positive = right up |
| 4-5 | FrontBackDiffMm | 39:32, 47:40 | int16 BE | 1 | -32768 to 32767 | mm | Height diff front-to-back. Computed: length_cm * 10 * tan(pitch_rad) |
| 6-7 | LeftRightDiffMm | 55:48, 63:56 | int16 BE | 1 | -32768 to 32767 | mm | Height diff left-to-right. Computed: width_cm * 10 * tan(roll_rad) |

#### CornerData (0x31, 8 bytes) — 500 ms cycle (2 Hz)

Per-corner height adjustments. Normalized so the lowest corner = 0.

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0-1 | FrontLeftMm | 7:0, 15:8 | uint16 BE | 0-65535 | mm | Front-left raise amount |
| 2-3 | FrontRightMm | 23:16, 31:24 | uint16 BE | 0-65535 | mm | Front-right raise amount |
| 4-5 | RearLeftMm | 39:32, 47:40 | uint16 BE | 0-65535 | mm | Rear-left raise amount |
| 6-7 | RearRightMm | 55:48, 63:56 | uint16 BE | 0-65535 | mm | Rear-right raise amount |

#### StatusData (0x32, 4 bytes) — 2000 ms cycle (0.5 Hz)

System status and BNO055 calibration levels.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | StatusFlags | 7:0 | uint8 bitmask | 0-255 | Bit 0 (0x01): IMU connected. Bit 1 (0x02): fully calibrated (sys=3). Bit 2 (0x04): leveling active |
| 1 | CalibrationPacked | 15:8 | uint8 packed | 0-255 | Bits 7-6: system (0-3). Bits 5-4: gyro. Bits 3-2: accel. Bits 1-0: mag. 3 = fully calibrated |
| 2 | MountingOrientation | 23:16 | uint8 | 0-2 | Current mounting: 0=floor, 1=left wall, 2=right wall |
| 3 | Reserved0 | 31:24 | uint8 | 0 | Reserved, always 0x00 |

---

## Bit Encoding Conventions

All TrailCurrent CAN messages use the following encoding conventions:

### Byte Ordering

All multi-byte values use **big-endian** (Motorola) byte order. The most significant byte is transmitted first (lowest byte index).

### Integer Types

- **Unsigned integers**: Standard big-endian encoding. 16-bit values span 2 bytes, 24-bit span 3 bytes, 32-bit span 4 bytes.
- **Signed integers**: Used for tilt/height data (Plateau module). Encoded as signed 16-bit big-endian with a scale factor (typically 0.01 for degrees).

### Whole + Decimal Split Pattern

Several modules (Ampline, Solstice) encode decimal values by splitting them into two separate bytes:
- Byte N: whole number part
- Byte N+1: decimal part (hundredths, range 0-99)
- **Reconstruction:** `value = whole + decimal / 100.0`

This pattern is used for: battery voltage, battery current, SOC percentage, panel voltage, and panel current.

### Sign Conventions

Negative values are indicated by a separate sign byte:
- **Standard:** 0 = positive, 1 = negative (used for current, panel current, latitude/longitude)
- **Wattage exception:** 0x00 = positive, 0xFF = negative (BatteryShuntData2 only)

### Bitmask Encoding

- **Door status (Picket):** Each bit represents one reed switch input. 1 = open, 0 = closed.
- **Relay status (Switchback):** Each bit represents one relay. 1 = ON (energized), 0 = OFF.
- **Status flags (Plateau):** Individual status bits packed into a single byte.
- **Calibration (Plateau):** Four 2-bit fields packed into one byte (bits 7-6, 5-4, 3-2, 1-0).

### Scaled Values

Some signals use a scale factor to provide fractional precision in an integer field:
- **Speed:** Raw value * 0.01 = knots
- **Course:** Raw value * 0.1 = degrees
- **Altitude:** Raw value * 0.01 = meters
- **Lat/Lon:** Raw value * 0.0001 = degrees
- **Pitch/Roll:** Raw value * 0.01 = degrees
- **Humidity:** Raw value * 0.01 = percent

---

## Message Timing

| CAN ID | Message | Cycle Time | Rate |
|--------|---------|-----------|------|
| 0x06-0x09 | GPS data (4 messages) | 1000 ms | 1 Hz |
| 0x0A-0x11 | Picket door status (per module) | 200 ms | 5 Hz |
| 0x1B | DeviceStatusReport (Torrent) | 33 ms | ~30 Hz |
| 0x1F | EnvironmentSensorData (Borealis) | 2000 ms | 0.5 Hz |
| 0x23-0x24 | Battery shunt data (Ampline) | 33 ms | ~30 Hz |
| 0x2C-0x2D | Solar MPPT data (Solstice) | 33 ms | ~30 Hz |
| 0x30-0x31 | Tilt/corner data (Plateau) | 500 ms | 2 Hz |
| 0x32 | Status data (Plateau) | 2000 ms | 0.5 Hz |
| 0x00, 0x15, 0x18, 0x1E, 0x20, 0x25-0x27 | Commands | Event-driven | On demand |
| 0x28-0x2A | Switchback status | Event-driven | On state change |

---

## DBC File

The authoritative machine-readable CAN database is `TrailCurrent.dbc` in the documentation repository root. This file can be loaded into CAN analysis tools (e.g., SavvyCAN, PCAN Explorer, python-can with cantools) for message decoding, monitoring, and simulation.

---

## Related Documentation

- [GLOSSARY.md](GLOSSARY.md) - CAN bus terminology
- [GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md) - CAN transceiver pin assignments
- [MQTT_TOPICS.md](MQTT_TOPICS.md) - MQTT mapping of CAN messages
- [../02_Hardware_Modules/](../02_Hardware_Modules/) - Module-specific details
