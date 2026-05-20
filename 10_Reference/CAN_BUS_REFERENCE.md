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

TrailCurrent messages are organized in two ranges:

- **Hardware-module range**: `0x00–0x42` (decimal 0–66) — the ESP32-class modules. Next available in this range: `0x05`, `0x12-0x14` (Switchback bridges Picket here — see PicketStatus8/9/10), `0x22`, `0x30–0x32` (freed when Plateau moved to `0x36-0x39`), `0x3B–0x3D`, then `0x43+`.
- **Playbill head-unit range**: `0x100–0x12F` (decimal 256–303) — three contiguous 16-ID blocks, one per Playbill instance. See the [Playbill multi-instance block](#playbill-multi-instance-block) section below.

| CAN ID (hex) | CAN ID (dec) | Message Name | DLC | Sender | Cycle Time |
|--------------|-------------|--------------|-----|--------|------------|
| 0x00 | 0 | OtaUpdateNotification | 3 | Headwaters | Event-driven |
| 0x01 | 1 | WifiConfigProvisioning | 8 | Headwaters | Event-driven (multi-frame sequence) |
| 0x02 | 2 | DiscoveryTrigger | 0 | Headwaters | Event-driven |
| 0x03 | 3 | DiscoveryReset | 3 | Headwaters | Event-driven |
| 0x04 | 4 | FirmwareVersionReport | 6 | All modules | Event-driven (boot) |
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
| 0x15 | 21 | TorrentBrightness0 | 2 | Any sender | Event-driven |
| 0x16 | 22 | TorrentBrightness1 | 2 | Any sender | Event-driven |
| 0x17 | 23 | TorrentBrightness2 | 2 | Any sender | Event-driven |
| 0x18 | 24 | TorrentToggle0 | 2 | Any sender | Event-driven |
| 0x19 | 25 | TorrentToggle1 | 2 | Any sender | Event-driven |
| 0x1A | 26 | TorrentToggle2 | 2 | Any sender | Event-driven |
| 0x1B | 27 | TorrentStatus0 | 8 | Torrent addr 0 | 33 ms |
| 0x1C | 28 | TorrentStatus1 | 8 | Torrent addr 1 | 33 ms |
| 0x1D | 29 | TorrentStatus2 | 8 | Torrent addr 2 | 33 ms |
| 0x1F | 31 | EnvironmentSensorData | 8 | Borealis | 1000 ms |
| 0x20 | 32 | BorealisSafetyData | 8 | Borealis | 1000 ms |
| 0x21 | 33 | BorealisCalibration | 2 | Headwaters | Event-driven |
| 0x23 | 35 | ShuntBasicData1 | 7 | Solstice | 33 ms |
| 0x24 | 36 | ShuntBasicData2 | 5 | Solstice | 33 ms |
| 0x25 | 37 | SwitchbackToggle0 | 2 | Any sender | Event-driven |
| 0x26 | 38 | SwitchbackToggle1 | 2 | Any sender | Event-driven |
| 0x27 | 39 | SwitchbackToggle2 | 2 | Any sender | Event-driven |
| 0x28 | 40 | SwitchbackStatus0 | 1 | Switchback addr 0 | Event-driven |
| 0x29 | 41 | SwitchbackStatus1 | 1 | Switchback addr 1 | Event-driven |
| 0x2A | 42 | SwitchbackStatus2 | 1 | Switchback addr 2 | Event-driven |
| 0x2B | 43 | ShuntExtLive | 6 | Solstice | 33 ms (reserved — zeros) |
| 0x2C | 44 | SolarMpptData1 | 7 | Solstice | 33 ms |
| 0x2D | 45 | SolarMpptData2 | 3 | Solstice | 33 ms |
| 0x2E | 46 | SolarLoadControl | 1 | Any sender | Event-driven |
| 0x2F | 47 | ShuntExtHistory | 6 | Solstice | 33 ms (reserved — zeros) |
| 0x33 | 51 | TorrentSequence0 | 1 | Any sender | Event-driven |
| 0x34 | 52 | TorrentSequence1 | 1 | Any sender | Event-driven |
| 0x35 | 53 | TorrentSequence2 | 1 | Any sender | Event-driven |
| 0x36 | 54 | LevelingConfig | 7 | Headwaters | Event-driven (Plateau firmware not yet shipping) |
| 0x37 | 55 | TiltData | 8 | Plateau | 500 ms (firmware not yet shipping) |
| 0x38 | 56 | CornerData | 8 | Plateau | 500 ms (firmware not yet shipping) |
| 0x39 | 57 | StatusData | 4 | Plateau | 2000 ms (firmware not yet shipping) |
| 0x3A | 58 | TrailerStatus | 3 | Aftline | 33 ms |
| 0x3E | 62 | WaterTankLevels | 3 | Reservoir | 2000 ms |
| 0x3F | 63 | ThermaDesiredTemperature | 3 | Therma | 1000 ms |
| 0x40 | 64 | ThermaStatus | 6 | Therma | 1000 ms |
| 0x41 | 65 | ThermaSetDesiredRequest | 2 | Any sender | Event-driven |
| 0x42 | 66 | ThermaSetThresholdRequest | 1 | Any sender | Event-driven |
| 0x100–0x12F | 256–303 | Playbill block (3 instances × 10 message types — see [Playbill multi-instance block](#playbill-multi-instance-block)) | 1–8 | Mixed (Any sender → Playbill; Playbill → bus for `*Status` and `Presence`) | Mixed |

### Bus Nodes

| Node | Platform | Description |
|------|----------|-------------|
| Bearing | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) | GPS receiver (DFRobot GNSS). Broadcasts GNSS position, altitude, speed, course, and date/time. ESP-IDF firmware |
| Torrent | ESP32 (WROOM) | 8-channel PWM power distribution module. Compile-time address 0-2, up to 3 on same bus. ESP-IDF firmware |
| Tapper | ESP32 (WROOM) | Physical 8-button control panel. Target-selectable at build time (Torrent or Switchback) with instance addressing. ESP-IDF firmware |
| Solstice | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) | Solar + battery gateway. Reads Victron MPPT via VE.Direct TEXT+HEX SET on UART1 and Victron SmartShunt via VE.Direct TEXT on UART2 (RX-only on current hardware). Transmits basic battery data (0x23/0x24), MPPT solar data (0x2C/0x2D). 0x2B/0x2F reserved (zeros). Accepts MPPT load control (0x2E). ESP-IDF firmware |
| Borealis | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) | Environment + safety sensor. Sensirion SCD41 (real CO2 + temp + humidity, photoacoustic NDIR) and Sensirion SGP40 (VOC Index 1-500) over I²C; DFRobot SEN0466 carbon-monoxide sensor (electrochemical, 0-1000 ppm) on I²C; DFRobot SEN0131 MQ-6 propane/LPG sensor on ADC. Transmits EnvironmentSensorData (0x1F) and BorealisSafetyData (0x20) at 1 Hz. ESP-IDF firmware |
| Picket | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) | Cabinet/door sensor. Up to 12 reed switch inputs, NVS-addressed (8 modules max). ESP-IDF firmware |
| Switchback | ESP32-S3 (Waveshare ESP32-S3-ETH-8DI-8RO-C) | 8-channel dry-contact relay module **plus** Picket-compatible 8-input sensor node on the same board. Relay control on 0x25-0x2A (per-instance); 8 digital inputs transmitted on 0x12-0x14 in PicketStatus8/9/10 wire format (consumed by the same Headwaters path as Picket). Up to 3 on same bus (compile-time address 0-2). ESP-IDF firmware |
| Plateau | ESP32-S3-Zero | Vehicle leveling with Adafruit BNO055 IMU; per-corner height calculation. ESP-IDF firmware |
| Aftline | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) | Trailer 7-pin connector monitor (ADC voltage + digital turn/brake/light sensing). ESP-IDF firmware |
| Therma | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) controller + 2× ESP32-S3 (Waveshare ESP32-S3-Relay-1CH) relay boards | Closed-loop thermostat. Controller owns authoritative desired temperature, mode, and threshold. Drives heater/cooler relay boards over direct GPIO (relay boards are not on the CAN bus). ESP-IDF firmware |
| Reservoir | ESP32-S3 (Waveshare ESP32-S3-RS485-CAN) | Water tank level monitor. Reads contactless sensors on up to 3 tanks (fresh/grey/black) and reports fill percentages. ESP-IDF firmware |
| Headwaters | Raspberry Pi Compute Module 5 (CM5) on standard carrier with Waveshare RS485 CAN HAT (B) | Dockerized edge gateway: CAN ↔ MQTT bridge, MQTT broker (Mosquitto), backend REST/WebSocket API, MongoDB, local tile server, and touchscreen dashboard. Host for OTA distribution and cloud sync |
| Playbill | Radxa Dragon Q6A (Qualcomm QCS6490) running Ubuntu Noble 24.04 + GNOME on Wayland | In-rig entertainment head. Up to three instances per rig, each binding to a 16-ID CAN block (`0x100–0x10F` / `0x110–0x11F` / `0x120–0x12F`). Receives nav/transport/radio/system/launch/volume commands; transmits transport/radio/screen status and a 60 s presence heartbeat. A Playbill with `device.canInstance = null` is MQTT-only and does not occupy a CAN block |

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

### WiFi Credential Provisioning (0x01)

**WifiConfigProvisioning** (8 bytes per frame, multi-frame sequence) — Sent by Headwaters to push WiFi credentials to every ESP-IDF module on the bus. The DLC is always 8; semantics of bytes 1-7 depend on byte 0 (`Subcommand`):

| Subcommand | Name | Byte 1 | Bytes 2-7 |
|---|---|---|---|
| `0x01` | Start | `ssid_len` | byte 2 = `pass_len`, byte 3 = `ssid_chunks` = ⌈ssid_len/6⌉, byte 4 = `pass_chunks` = ⌈pass_len/6⌉, bytes 5-7 zero |
| `0x02` | SSID chunk | `chunk_index` | up to 6 bytes of SSID text, zero-padded |
| `0x03` | Password chunk | `chunk_index` | up to 6 bytes of password text, zero-padded |
| `0x04` | End | `xor_checksum` (XOR of all SSID + password bytes) | zero-padded |

Headwaters sends the frames with a 50 ms inter-frame delay. Receivers buffer chunks indexed by chunk number, validate the checksum on the End frame, and commit the credentials to NVS on success. Every ESP-IDF module defines `CAN_ID_WIFI_CONFIG = 0x01` and listens for this. Implementation: [`TrailCurrentHeadwaters/containers/backend/src/mqtt.js:980-1043`](../../TrailCurrentHeadwaters/containers/backend/src/mqtt.js#L980-L1043).

---

### Module Discovery (0x02, 0x03)

Self-discovery protocol for automatic module registration with Headwaters.

#### DiscoveryTrigger (0x02, 0 bytes)

Broadcast by Headwaters with no payload. All unconfigured modules respond by connecting to WiFi and advertising a `_trailcurrent._tcp` mDNS service with TXT records:

| TXT Key | Example Value | Description |
|---------|---------------|-------------|
| `type`  | `picket`      | Module type (compile-time) |
| `addr`  | `3`           | Instance address (compile-time) |
| `canid` | `0x0D`        | CAN TX message ID for this instance (used by Headwaters to route data and commands) |
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

### Firmware Version Report (0x04)

**FirmwareVersionReport** (6 bytes) — Sent by every module once on boot after CAN/TWAI initialization. Reports the running firmware version so Headwaters can track what each device is actually running. This is the mechanism that confirms firmware version after an OTA update — the device reboots with new firmware and broadcasts its version on startup.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | MacAddressByte4 | 7:0 | uint8 | 0-255 | 4th byte of device WiFi MAC (same as hostname `esp32-XXYYZZ` byte XX) |
| 1 | MacAddressByte5 | 15:8 | uint8 | 0-255 | 5th byte of device WiFi MAC (YY) |
| 2 | MacAddressByte6 | 23:16 | uint8 | 0-255 | 6th byte of device WiFi MAC (ZZ) |
| 3 | VersionMajor | 31:24 | uint8 | 0-255 | Semantic version major number |
| 4 | VersionMinor | 39:32 | uint8 | 0-255 | Semantic version minor number |
| 5 | VersionPatch | 47:40 | uint8 | 0-255 | Semantic version patch number |

The version is read from the ESP-IDF app descriptor (`esp_app_get_description()->version`), which is embedded in the firmware binary at build time from `PROJECT_VER` in CMakeLists.txt. Headwaters CAN bridge matches the MAC bytes to a registered module hostname and updates the `fw` field in MongoDB.

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

Monitors up to 12 magnetic reed switch inputs per module. Uses the Waveshare ESP32-S3-RS485-CAN board with onboard TJA1051 CAN transceiver. Module address (0-7) is set at compile time via `PICKET_ADDRESS` build flag (`idf.py build -DPICKET_ADDRESS=N`), allowing up to 8 Picket modules on the same bus. CAN IDs 0x0A through 0x11 share identical signal layout. Sent at 5 Hz (200 ms cycle). All reed switch inputs use internal pull-ups with no external resistors required.

> The Picket address pool extends through `0x14` — IDs `0x12/0x13/0x14` are reserved for [Switchback](#switchback---relay-control-module--sensor-input-node-0x12-0x14-0x25-0x2a) modules broadcasting their 8 digital inputs in the same PicketStatus wire format. Headwaters and any other Picket consumer treat those frames identically to Picket's own.

#### PicketStatus (0x0A-0x11, 2 bytes each)

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | DoorStatus1to8 | 7:0 | uint8 bitmask | 0-255 | Bit 0 = door 1 (RSW01) ... bit 7 = door 8 (RSW08). 1 = open, 0 = closed |
| 1 | DoorStatus9to12 | 15:8 | uint8 bitmask | 0-15 | Bits 0-3 = doors 9-12 (RSW09-RSW12). Bits 4-7 reserved |

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

### Borealis - Environment + Safety Sensor (0x1F, 0x20)

Combined environment and safety sensor module (Waveshare ESP32-S3-RS485-CAN). Reads four sensors and broadcasts two CAN frames at 1 Hz:

- **SCD41** (DFRobot SEN0536) over I²C — photoacoustic NDIR; provides **real CO2** (not eCO2), temperature, and humidity from a single sensor
- **SGP40** (DFRobot SEN0394) over I²C — VOC Index 1-500 (relative trending; 100 = running average)
- **SEN0466** electrochemical carbon-monoxide sensor over I²C — factory-calibrated, 0-1000 ppm
- **SEN0131** MQ-6 propane/LPG sensor on ADC1_CH2 — reports Rs/R0 ratio (lower = more gas)

> **Wire-format change**: bytes 4-7 of `EnvironmentSensorData` changed when Borealis moved from the DHT22 + SGP30 generation to the SCD41 + SGP40 generation. The byte positions are the same; the signal *semantics* are different. Bytes 4-5 are now **real CO2 ppm** (not TVOC ppb) and bytes 6-7 are now **VOC Index 1-500** (not eCO2 ppm). Any consumer decoding the old layout will return nonsense — re-decode from the current DBC.

#### EnvironmentSensorData (0x1F, 8 bytes) — 1000 ms cycle (1 Hz)

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0 | TemperatureCelsius | 7:0 | uint8 | 1 | -40 to 125 | degC | Whole degrees Celsius (SCD41) |
| 1 | TemperatureFahrenheit | 15:8 | uint8 | 1 | -40 to 257 | degF | Whole degrees Fahrenheit: F = (C * 9 + 3) / 5 + 32 |
| 2-3 | HumidityScaled | 23:16, 31:24 | uint16 BE | 0.01 | 0-100 | % | Relative humidity from SCD41 (raw / 100 = %). E.g., 5523 = 55.23% |
| 4-5 | CO2Ppm | 39:32, 47:40 | uint16 BE | 1 | 400-40000 | ppm | Real CO2 from SCD41 NDIR. <600 Excellent, 600-1000 Good, 1000-1500 Moderate, 1500-2500 Poor, >2500 Unhealthy |
| 6-7 | VOCIndex | 55:48, 63:56 | uint16 BE | 1 | 1-500 | (index) | Sensirion VOC Index from SGP40. 100 = running average; >100 means more VOCs than recent baseline. >400 typically alarmable |

#### BorealisSafetyData (0x20, 8 bytes) — 1000 ms cycle (1 Hz)

Combined alarm channel covering all four hazards on a single ID. Borealis evaluates thresholds on-board so a consumer only needs to read byte 4 to know the current alarm state.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0-1 | CarbonMonoxidePpm | 7:0, 15:8 | uint16 BE | 1 | 0-1000 | ppm | CO concentration from SEN0466 electrochemical sensor |
| 2-3 | LpgRatioScaled | 23:16, 31:24 | uint16 BE | 0.001 | 0.000-65.535 | (ratio) | Propane/LPG Rs/R0 ratio from MQ-6 (SEN0131). Lower values mean more gas present |
| 4 | AlarmFlags | 39:32 | uint8 (bitmask) | — | 0-255 | (flags) | Bitmask: bit 0 CO warn, bit 1 CO alarm, bit 2 LPG warn, bit 3 LPG alarm, bit 4 CO2 warn, bit 5 CO2 alarm, bit 6 VOC alarm |
| 5-7 | (reserved) | — | — | — | — | — | Zero-padded |

**Alarm thresholds** (compiled into firmware): CO alarm ≥ 200 ppm / warn ≥ 70 ppm; LPG alarm Rs/R0 < 0.3 / warn < 0.5; CO2 alarm ≥ 2500 ppm / warn ≥ 1500 ppm (evaluated against the value sent on `EnvironmentSensorData`); VOC alarm at VOC Index ≥ 400.

**Consumer rule**: byte 4 (`AlarmFlags`) is the source of truth for current alarm state. Bytes 0-3 are for trending/display only — do not threshold them in consumer code, because the firmware may change its own thresholds in a later release.

---

### Aftline - Trailer 7-Pin Connector Monitor (0x3A)

Aftline monitors the standard 7-pin trailer connector. It reads the four signal lines (left turn, right turn, brake, running lights) via opto-isolated GPIOs and the trailer-side battery voltage via an ADC divider. Transmits one frame.

#### TrailerStatus (0x3A, 3 bytes) — 33 ms cycle (~30 Hz)

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0 | Connected     | 7:7 | bool | 0-1 | — | 1 = any voltage present on the trailer pigtail |
| 0 | LeftTurn      | 6:6 | bool | 0-1 | — | 1 = left turn signal active |
| 0 | RightTurn     | 5:5 | bool | 0-1 | — | 1 = right turn signal active |
| 0 | RunningLights | 4:4 | bool | 0-1 | — | 1 = running/marker lights active |
| 0 | Brakes        | 3:3 | bool | 0-1 | — | 1 = brake lights active |
| 1-2 | TrailerVoltageMv | 15:8, 23:16 | uint16 BE | 0-65535 | mV | Trailer-side supply voltage (ADC reading through resistor divider) |

> Aftline previously transmitted on `0x10`, which collided with `PicketStatus6` in Picket's reserved `0x0A-0x14` range. It was moved to `0x3A` (2026-05-19) — Aftline firmware updated to match.

---

### Solstice - SmartShunt Basic Battery Data (0x23-0x24)

Transmitted by Solstice. Reads Victron SmartShunt VE.Direct TEXT fields over UART2 (19200 baud, RX-only on current hardware) and converts to CAN messages. Both messages sent at ~30 Hz (33 ms cycle).

#### ShuntBasicData1 (0x23, 7 bytes)

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

#### ShuntBasicData2 (0x24, 5 bytes)

Power consumption and time remaining.

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0 | IsWattageNegative | 7:0 | uint8 | 0/255 | - | 0x00 = charging, 0xFF = discharging. Note: uses 0xFF, not 1 |
| 1-2 | Wattage | 15:8, 23:16 | uint16 BE | 0-65535 | W | Absolute power in watts |
| 3-4 | TimeToGo | 31:24, 39:32 | uint16 BE | 0-65535 | min | Minutes until battery depleted (65535 may = infinite/charging) |

**Example:** Bytes `0xFF, 0x01, 0x2C, 0x02, 0x58` = -300W discharge, 600 minutes remaining.

---

### Switchback - Relay Control Module + Sensor Input Node (0x12-0x14, 0x25-0x2A)

8-channel dry-contact relay control module **plus** a Picket-compatible 8-input sensor node on the same board. ESP32-S3 on a Waveshare ESP32-S3-ETH-8DI-8RO-C — 8 relay outputs (8RO) and 8 opto-isolated digital inputs (8DI). Relays are binary on/off (not PWM). The same physical Switchback drives relays and reports sensor inputs simultaneously; there's no mode switch. Supports up to 3 modules on the same bus, each instance owning one slot in *both* the relay pool and the input pool.

**Two-pool addressing**:

| Switchback address | Relay toggle (RX) | Relay status (TX) | Sensor input (TX) |
|---|---|---|---|
| 0 | 0x25 | 0x28 | 0x12 (PicketStatus8) |
| 1 | 0x26 | 0x29 | 0x13 (PicketStatus9) |
| 2 | 0x27 | 0x2A | 0x14 (PicketStatus10) |

The sensor-input frames use the same wire format as Picket's `0x0A-0x11` range — Headwaters consumes them through the same code path as any Picket module, so no Switchback-specific decoder is needed. See the [Picket section](#picket---cabinet--door-sensors-0x0a-0x11) above for the byte layout. Switchback always sets byte 1 to `0x00` because its hardware has 8 inputs vs Picket's 12.

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

Reads Victron MPPT solar charge controller data via VE.Direct TEXT on UART1 (19200 baud, bidirectional — TEXT parsing + HEX SET for load control). Both messages sent at ~30 Hz (33 ms cycle). Solstice also transmits SmartShunt basic battery data on 0x23/0x24 (see earlier section).

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

### Plateau - Vehicle Leveler (0x36-0x39)

> ℹ️ **Plateau firmware is not yet shipping.** The byte layouts below match Plateau's current source code (`/Product/TrailCurrentPlateau/`), which has been updated to the new ID block. Consumers integrating against the DBC can wire to these IDs now; they will not move again before Plateau ships.

Vehicle leveling module (ESP32-S3-Zero with Adafruit BNO055 9-DOF IMU). Reads pitch and roll, computes per-corner height adjustments based on configurable vehicle dimensions. Supports three mounting orientations with automatic BNO055 axis remapping. Calibration offsets auto-saved to NVS when fully calibrated.

Plateau's CAN IDs were reallocated 2026-05-19 from the original `0x20 / 0x30 / 0x31 / 0x32` to the contiguous `0x36-0x39` block when Borealis claimed `0x20` for its safety frame. Both DBC and firmware were updated together; no consumer should be wired to the legacy IDs.

#### LevelingConfig (0x36, 7 bytes) — Event-driven command

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
| 2 | Request Status | Request immediate status report on CAN ID 0x39 |
| 3 | Zero/Tare | Calibration reset (reserved for future use) |

**MountingSurface values:**

| Value | Orientation | Description |
|-------|-------------|-------------|
| 0 | Floor | Horizontal mounting (default) |
| 1 | Left Wall | Vertical, left side |
| 2 | Right Wall | Vertical, right side |

#### TiltData (0x37, 8 bytes) — 500 ms cycle (2 Hz)

Vehicle pitch, roll, and computed height differences.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0-1 | PitchScaled | 7:0, 15:8 | int16 BE | 0.01 | -327.68 to 327.67 | deg | Pitch (raw / 100). Positive = front up |
| 2-3 | RollScaled | 23:16, 31:24 | int16 BE | 0.01 | -327.68 to 327.67 | deg | Roll (raw / 100). Positive = right up |
| 4-5 | FrontBackDiffMm | 39:32, 47:40 | int16 BE | 1 | -32768 to 32767 | mm | Height diff front-to-back. Computed: length_cm * 10 * tan(pitch_rad) |
| 6-7 | LeftRightDiffMm | 55:48, 63:56 | int16 BE | 1 | -32768 to 32767 | mm | Height diff left-to-right. Computed: width_cm * 10 * tan(roll_rad) |

#### CornerData (0x38, 8 bytes) — 500 ms cycle (2 Hz)

Per-corner height adjustments. Normalized so the lowest corner = 0.

| Byte | Signal | Bits | Type | Range | Unit | Description |
|------|--------|------|------|-------|------|-------------|
| 0-1 | FrontLeftMm | 7:0, 15:8 | uint16 BE | 0-65535 | mm | Front-left raise amount |
| 2-3 | FrontRightMm | 23:16, 31:24 | uint16 BE | 0-65535 | mm | Front-right raise amount |
| 4-5 | RearLeftMm | 39:32, 47:40 | uint16 BE | 0-65535 | mm | Rear-left raise amount |
| 6-7 | RearRightMm | 55:48, 63:56 | uint16 BE | 0-65535 | mm | Rear-right raise amount |

#### StatusData (0x39, 4 bytes) — 2000 ms cycle (0.5 Hz)

System status and BNO055 calibration levels.

| Byte | Signal | Bits | Type | Range | Description |
|------|--------|------|------|-------|-------------|
| 0 | StatusFlags | 7:0 | uint8 bitmask | 0-255 | Bit 0 (0x01): IMU connected. Bit 1 (0x02): fully calibrated (sys=3). Bit 2 (0x04): leveling active |
| 1 | CalibrationPacked | 15:8 | uint8 packed | 0-255 | Bits 7-6: system (0-3). Bits 5-4: gyro. Bits 3-2: accel. Bits 1-0: mag. 3 = fully calibrated |
| 2 | MountingOrientation | 23:16 | uint8 | 0-2 | Current mounting: 0=floor, 1=left wall, 2=right wall |
| 3 | Reserved0 | 31:24 | uint8 | 0 | Reserved, always 0x00 |

---

### Therma - Closed-Loop Thermostat (0x3F-0x42)

Three-board thermostat. Only the **controller** (Waveshare ESP32-S3-RS485-CAN) is on the CAN bus; it owns the authoritative desired temperature, hysteresis threshold, and heat/cool mode for the bus, and drives two **Waveshare ESP32-S3-Relay-1CH** relay boards (heater and cooler) over direct GPIO pin-to-pin lines. Heating and cooling outputs are mutually exclusive and enforced in the controller.

**N-way setpoint pattern.** Any device on the bus (PWA, wall panels, Farwatch cloud, Headwaters) can request a change via `ThermaSetDesiredRequest` (0x41) or `ThermaSetThresholdRequest` (0x42). Therma validates, clamps to safe range, persists to NVS, and broadcasts the new authoritative value on `ThermaDesiredTemperature` (0x3F) on the next tick. **Displays must always show the value from Therma's broadcast, never a locally-held copy** — this is the same pattern used by Torrent/Switchback for their authoritative state.

**Current temperature source.** The controller consumes `EnvironmentSensorData` (0x1F) from Borealis and uses `TemperatureCelsius` (byte 0) as the control-loop input, normalized to 0.1 °C units internally.

#### ThermaDesiredTemperature (0x3F, 3 bytes) — 1000 ms cycle (1 Hz)

Broadcast by Therma at 1 Hz and immediately on change.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0-1 | SetpointDeciC | 7:0, 15:8 | int16 BE | 0.1 | -40 to 125 (accepted: 5.0 to 35.0) | degC | Desired temperature in tenths of a degree |
| 2 | ThresholdDeciC | 23:16 | uint8 | 0.1 | 0.1 to 5.0 | degC | Hysteresis deadband width around setpoint |

#### ThermaStatus (0x40, 6 bytes) — 1000 ms cycle (1 Hz)

Broadcast by Therma at 1 Hz. Single source of truth for the current thermostat mode on the bus.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0 | ThermaMode | 7:0 | uint8 | 1 | 0-2 | - | 0 = Idle, 1 = Heating, 2 = Cooling (mutually exclusive) |
| 1-2 | CurrentDeciC | 15:8, 23:16 | int16 BE | 0.1 | -40 to 125 | degC | Current control-loop temperature from Borealis |
| 3 | HeatRelayFb | 31:24 | uint8 | 1 | 0-1 | - | Heater relay feedback pin state |
| 4 | CoolRelayFb | 39:32 | uint8 | 1 | 0-1 | - | Cooler relay feedback pin state |
| 5 | FaultBitmask | 47:40 | uint8 bitmask | 1 | 0-255 | - | See fault bits below |

**ThermaMode values:**

| Value | Mode | Description |
|-------|------|-------------|
| 0 | Idle | Both outputs off (default at boot, during faults, and when in the deadband) |
| 1 | Heating | Heater relay commanded on, cooler off |
| 2 | Cooling | Cooler relay commanded on, heater off |

**FaultBitmask bits:**

| Bit | Mask | Name | Description |
|-----|------|------|-------------|
| 0 | 0x01 | SensorStale | No `EnvironmentSensorData` (0x1F) received for more than 10 seconds. Controller drops to Idle as a fail-safe |
| 1 | 0x02 | HeatFbMismatch | Heater relay feedback pin does not match commanded heating state |
| 2 | 0x04 | CoolFbMismatch | Cooler relay feedback pin does not match commanded cooling state |
| 3-7 | - | Reserved | Always 0 |

#### ThermaSetDesiredRequest (0x41, 2 bytes) — Event-driven

Request to change the desired temperature. Sent by any device on the bus. Therma validates (clamps to [5.0, 35.0] °C), persists to NVS, and re-broadcasts the new authoritative value on `ThermaDesiredTemperature` (0x3F) on the next tick. Rejected requests are silently ignored. Senders must **not** display the requested value locally.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0-1 | RequestedSetpointDeciC | 7:0, 15:8 | int16 BE | 0.1 | -40 to 125 (accepted: 5.0 to 35.0) | degC | Requested new desired temperature |

#### ThermaSetThresholdRequest (0x42, 1 byte) — Event-driven

Request to change the hysteresis threshold (deadband). Same pattern as `ThermaSetDesiredRequest`.

| Byte | Signal | Bits | Type | Scale | Range | Unit | Description |
|------|--------|------|------|-------|-------|------|-------------|
| 0 | RequestedThresholdDeciC | 7:0 | uint8 | 0.1 | 0.1 to 5.0 | degC | Requested new deadband width |

---

### Playbill multi-instance block

Playbill is the rig-side entertainment head (Radxa Dragon Q6A running a GNOME desktop with an Electron app). It is the only TrailCurrent device that uses CAN IDs above `0x42`, and it is the only device that allocates **multiple contiguous blocks** of CAN IDs — one per Playbill instance, up to three per rig.

The narrative source for this section is [Playbill `docs/app/dbc-additions.md`](../../TrailCurrentPlaybill/docs/app/dbc-additions.md). The wire-level contract for a third-party MCU that wants to drive a Playbill is [Headwaters `DOCS/CAN-REMOTE.md`](../../TrailCurrentHeadwaters/DOCS/CAN-REMOTE.md). If anything here disagrees with the DBC, the DBC wins.

#### Addressing

Each Playbill instance claims one of three CAN address blocks at install time via its `device.canInstance` setting:

| `canInstance` | Block         | What it controls |
|---|---|---|
| `0` | `0x100 – 0x10F` | Playbill 0 (default name "Living Room") |
| `1` | `0x110 – 0x11F` | Playbill 1 (default name "Bedroom") |
| `2` | `0x120 – 0x12F` | Playbill 2 (default name "Bunkhouse") |
| `null` | (none) | MQTT-only — does not participate in CAN |

Every message type uses the **same offset within the block**. Address math:

```
target_id = (0x100 + 0x10 * instance) + offset
```

Reserved offsets `+0xA … +0xF` are intentionally empty so the message set can grow within each block without renumbering.

A fourth Playbill instance is a future expansion (`0x130–0x13F`) — call it out before doing it.

#### Message map (per instance)

| Offset | Message | Size | Direction | Cycle Time |
|---|---|---|---|---|
| `+0x0` | `PlaybillNavCmd<N>` | 1 | → Playbill | Event-driven |
| `+0x1` | `PlaybillTransportCmd<N>` | 5 | → Playbill | Event-driven |
| `+0x2` | `PlaybillTransportStatus<N>` | 8 | ← Playbill | 500–1000 ms (edge-triggered) |
| `+0x3` | `PlaybillRadioTuneReq<N>` | 6 | → Playbill | Event-driven |
| `+0x4` | `PlaybillRadioStatus<N>` | 8 | ← Playbill | 500–1000 ms (edge-triggered) |
| `+0x5` | `PlaybillScreenStatus<N>` | 2 | ← Playbill | 500–1000 ms (edge-triggered) |
| `+0x6` | `PlaybillSystemCmd<N>` | 1 | → Playbill | Event-driven |
| `+0x7` | `PlaybillLaunchSourceCmd<N>` | 2 | → Playbill | Event-driven |
| `+0x8` | `PlaybillVolumeCmd<N>` | 2 | → Playbill | Event-driven |
| `+0x9` | `PlaybillPresence<N>` | 6 | ← Playbill | 60 000 ms (heartbeat) |
| `+0xA … +0xF` | RESERVED | — | — | — |

All payloads are byte-aligned, Motorola big-endian (matching every existing TrailCurrent message). 8-byte CAN 2.0A frames; messages shorter than 8 bytes leave trailing bytes unused.

#### PlaybillNavCmd (`+0x0`) — D-pad / remote keys

| Byte | Bits | Signal | Enum |
|---|---|---|---|
| 0 | `7\|8` | `NavKey` | 0=Up · 1=Down · 2=Left · 3=Right · 4=Select · 5=Back · 6=Home · 7=Menu |

**Auto-wake behavior**: if no Electron GUI is currently connected to the Playbill controller, the *first* `NavCmd` frame triggers `system.launchGui` automatically and is **not** delivered as a navigation event. Subsequent frames navigate normally. Treat like the first press on an Apple-TV-style remote — it wakes the box.

CAN IDs: `0x100` / `0x110` / `0x120`.

#### PlaybillTransportCmd (`+0x1`) — Play / pause / seek

| Byte | Bits | Signal | Notes |
|---|---|---|---|
| 0 | `7\|8` | `Action` | 0=Play · 1=Pause · 2=Stop · 3=Toggle · 4=SeekRel · 5=SeekAbs · 6=Next · 7=Previous |
| 1–4 | `15\|32` | `Value` | 32-bit BE. SeekRel: signed-as-unsigned ms (`0x80000000 + delta_ms`). SeekAbs: absolute ms. Unused for the other actions — send zeros. |

Volume and mute are deliberately split into `PlaybillVolumeCmd` (`+0x8`) so a hardware volume encoder can wire to a single CAN ID with no enum-parsing logic.

CAN IDs: `0x101` / `0x111` / `0x121`.

#### PlaybillTransportStatus (`+0x2`) — What's playing right now

| Bits | Signal | Range | Meaning |
|---|---|---|---|
| `7\|1` | `Paused` | 0–1 | 1 if paused or stopped |
| `6\|1` | `Muted` | 0–1 | |
| `5\|6` | `SourceEnum` | 0–63 | enum below |
| `15\|8` | `VolumePct` | 0–100 (255=unknown) | percent |
| `23\|24` | `PositionSec` | 0–16 777 215 | current playback position in seconds |
| `47\|24` | `DurationSec` | 0–16 777 215 (0=live/unknown) | total duration |

**`SourceEnum`** (shared by TransportStatus and LaunchSourceCmd):
`0=None · 1=YouTube · 2=LiveTV · 3=Radio · 4=LocalLibrary · 5=Plex · 6=Spotify · 7=Netflix · 8–63 reserved`

Status messages are **edge-triggered** (republished only when state changes). A consumer that needs a synchronous read should cache the last value and re-request via the matching command.

Text titles, artwork URLs, etc. live on the MQTT-only `local/playbill/<id>/now-playing` topic — not on CAN.

CAN IDs: `0x102` / `0x112` / `0x122`.

#### PlaybillRadioTuneReq (`+0x3`) — Tune the RTL-SDR radio

Frequency in **kilohertz** as a 32-bit unsigned int — covers FM (88 000 – 108 000 kHz), AM (530 – 1700 kHz), and the public-safety scanner range (up to 1 GHz) without a band-multiplier signal.

| Byte | Bits | Signal | Notes |
|---|---|---|---|
| 0 | `7\|8` | `Band` | 0=FM · 1=AM · 2=Scanner |
| 1–4 | `15\|32` | `FrequencyKHz` | kHz |
| 5 | `47\|8` | `Mode` | reserved (future NFM/WFM/AM modulation override) |

CAN IDs: `0x103` / `0x113` / `0x123`.

#### PlaybillRadioStatus (`+0x4`) — Current radio state

| Bits | Signal | Range | Meaning |
|---|---|---|---|
| `7\|8` | `Band` | 0–2 | same enum as TuneReq |
| `15\|32` | `FrequencyKHz` | 0–4 294 967 295 | kHz |
| `47\|8` | `SignalDbm` (signed) | −128 to +127 | dBm |
| `55\|1` | `Tuned` | 0–1 | 1 if currently tuned & demodulating |
| `54\|1` | `Scanning` | 0–1 | 1 if mid-scan |

CAN IDs: `0x104` / `0x114` / `0x124`.

#### PlaybillScreenStatus (`+0x5`) — Which screen the GUI is on

| Bits | Signal | Range | Meaning |
|---|---|---|---|
| `7\|8` | `ScreenEnum` | 0–255 | 0=Home · 1=Apps · 2=Live · 3=Radio · 4=LocalLibrary · 5=Rig · 6=Settings · 7=NowPlaying · 8–255 reserved |
| `15\|1` | `GuiOpen` | 0–1 | 1 = Electron window is on screen |

Useful for a dash status LED or for the PWA to mirror current focus.

CAN IDs: `0x105` / `0x115` / `0x125`.

#### PlaybillSystemCmd (`+0x6`) — Power / window lifecycle

| Byte | Bits | Signal | Enum |
|---|---|---|---|
| 0 | `7\|8` | `SysAction` | 0=LaunchGui · 1=QuitGui · 2=Focus · 3=Wake · 4=Sleep · 5–255 reserved |

`LaunchGui` is the explicit "power on" — use this from a dedicated power button on your remote if you don't want to rely on the nav-press auto-wake.

CAN IDs: `0x106` / `0x116` / `0x126`.

#### PlaybillLaunchSourceCmd (`+0x7`) — "Open this source"

Analogous to a hardware preset button on a stereo head unit. Implies LaunchGui + navigate.

| Byte | Bits | Signal | Enum |
|---|---|---|---|
| 0 | `7\|8` | `SourceEnum` | shared with TransportStatus |
| 1 | `15\|8` | `SubScreenEnum` | 0=Default · 1=SignIn · 2=Settings · 3=Search · 4–255 reserved |

CAN IDs: `0x107` / `0x117` / `0x127`.

#### PlaybillVolumeCmd (`+0x8`) — Volume + mute

Split out from TransportCmd so a hardware volume encoder or mute button can wire to a single CAN ID with no enum-parsing logic.

| Byte | Bits | Signal | Notes |
|---|---|---|---|
| 0 | `7\|8` | `VolAction` | 0=Up · 1=Down · 2=Set · 3=MuteOn · 4=MuteOff · 5=MuteToggle |
| 1 | `15\|8` | `Value` | Up/Down: step percent (1–100, 0 = default 5). Set: target percent 0–100. Mute actions: ignored. |

CAN IDs: `0x108` / `0x118` / `0x128`.

#### PlaybillPresence (`+0x9`) — Heartbeat (read-only)

Mirrors the `FirmwareVersionReport` (`0x004`) shape: last three bytes of the host's primary NIC MAC + version triplet. Sent once on startup and again every 60 s. Lets a CAN consumer (dash module, future status LED) discover which Playbills are alive on the bus without subscribing to MQTT.

| Byte | Bits | Signal | Notes |
|---|---|---|---|
| 0 | `7\|8` | `MacAddressByte4` | last three bytes of the host's primary NIC MAC |
| 1 | `15\|8` | `MacAddressByte5` | |
| 2 | `23\|8` | `MacAddressByte6` | |
| 3 | `31\|8` | `VersionMajor` | from controller `package.json` |
| 4 | `39\|8` | `VersionMinor` | |
| 5 | `47\|8` | `VersionPatch` | |

CAN IDs: `0x109` / `0x119` / `0x129`.

#### Cross-cutting design notes

- **CAN does not bleed into MQTT.** MQTT topics use the human-readable `device.id` slug (e.g. `local/playbill/living-room/...`); CAN uses the numeric `device.canInstance` (0/1/2). They're independent fields in `settings.json`.
- **`Vector__XXX` as transmitter** is used for `→ Playbill` messages because there's no single canonical sender — a touchscreen, a CAN button MCU, or the Headwaters CAN bridge can all originate them.
- **Reserved enum values and offset slots are forever.** Once a downstream device wires to (say) `SysAction = 3 = Wake`, that value cannot be reused. Add new values, don't recycle.

---

## Bit Encoding Conventions

All TrailCurrent CAN messages use the following encoding conventions:

### Byte Ordering

All multi-byte values use **big-endian** (Motorola) byte order. The most significant byte is transmitted first (lowest byte index).

### Integer Types

- **Unsigned integers**: Standard big-endian encoding. 16-bit values span 2 bytes, 24-bit span 3 bytes, 32-bit span 4 bytes.
- **Signed integers**: Used for tilt/height data (Plateau module). Encoded as signed 16-bit big-endian with a scale factor (typically 0.01 for degrees).

### Whole + Decimal Split Pattern

Solstice encodes decimal values (for both SmartShunt basic data on 0x23/0x24 and MPPT solar data on 0x2C/0x2D) by splitting them into two separate bytes:
- Byte N: whole number part
- Byte N+1: decimal part (hundredths, range 0-99)
- **Reconstruction:** `value = whole + decimal / 100.0`

This pattern is used for: battery voltage, battery current, SOC percentage, panel voltage, and panel current.

### Sign Conventions

Negative values are indicated by a separate sign byte:
- **Standard:** 0 = positive, 1 = negative (used for current, panel current, latitude/longitude)
- **Wattage exception:** 0x00 = positive, 0xFF = negative (ShuntBasicData2 only)

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
| 0x23-0x24 | SmartShunt basic data (Solstice) | 33 ms | ~30 Hz |
| 0x2B | ShuntExtLive (Solstice, reserved — transmits zeros) | 33 ms | ~30 Hz |
| 0x2C-0x2D | Solar MPPT data (Solstice) | 33 ms | ~30 Hz |
| 0x2F | ShuntExtHistory (Solstice, reserved — transmits zeros) | 33 ms | ~30 Hz |
| 0x37-0x38 | Tilt/corner data (Plateau) | 500 ms | 2 Hz |
| 0x39 | Status data (Plateau) | 2000 ms | 0.5 Hz |
| 0x3F | ThermaDesiredTemperature | 1000 ms | 1 Hz |
| 0x40 | ThermaStatus | 1000 ms | 1 Hz |
| 0x00, 0x15, 0x18, 0x1E, 0x20, 0x25-0x27, 0x41, 0x42 | Commands | Event-driven | On demand |
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
