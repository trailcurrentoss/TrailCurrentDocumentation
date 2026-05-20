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

### TrailCurrent Borealis (Environment & Safety)
- **Purpose**: Monitor temperature, humidity, real CO2, VOC Index, **carbon monoxide, and propane/LPG** for both comfort and life-safety
- **Hardware**: Waveshare ESP32-S3-RS485-CAN (ESP32-S3R8, 8 MB PSRAM, 16 MB flash, isolated TJA1051 CAN transceiver, PCF85063AT RTC, 7-36 V DC or 5 V USB-C input)
- **Framework**: ESP-IDF
- **Inputs**: Sensirion SCD41 (photoacoustic NDIR — real CO2 + temp + humidity, via DFRobot SEN0536) + Sensirion SGP40 (VOC Index 1-500, via DFRobot SEN0394) + DFRobot SEN0466 carbon monoxide (electrochemical, factory-calibrated, 0-1000 ppm) all over I²C; DFRobot SEN0131 MQ-6 propane/LPG on ADC1_CH2 via 10 kΩ/15 kΩ divider
- **Outputs**: Two CAN frames at 1 Hz — `EnvironmentSensorData` (0x1F) and `BorealisSafetyData` (0x20)
- **CAN IDs**: 0x1F (EnvironmentSensorData), 0x20 (BorealisSafetyData — CO/LPG/alarm flags); accepts 0x21 (BorealisCalibration)
- **Safety thresholds (compile-time)**: CO alarm ≥ 200 ppm / warn ≥ 70 ppm; LPG alarm Rs/R0 < 0.3 / warn < 0.5; CO2 alarm ≥ 2500 ppm / warn ≥ 1500 ppm; VOC alarm at index ≥ 400. Borealis evaluates these on-board so a consumer only needs to read the alarm-flags byte to know current state.
- **Documentation**: [Borealis.md](Borealis.md)
- **Source**: `/Product/TrailCurrentBorealis/`

### TrailCurrent Picket (Cabinet & Door Sensors)
- **Purpose**: Monitor open/closed status of doors, windows, cabinets, and bays
- **Hardware**: Waveshare ESP32-S3-RS485-CAN (off-the-shelf board with onboard CAN transceiver)
- **Framework**: ESP-IDF
- **Inputs**: Up to 12 magnetic reed switches (via pin header, internal pull-ups, no external resistors)
- **Outputs**: CAN messages with door/cabinet state
- **Addressing**: Compile-time `PICKET_ADDRESS` build flag (0-7, up to 8 modules per bus)
- **CAN ID Range**: 0x0A-0x11
- **Documentation**: [Picket.md](Picket.md)
- **Source**: `/Product/TrailCurrentPicket/`

### TrailCurrent Plateau (Vehicle Level Sensor)
> ℹ️ **Firmware not yet shipping.** Plateau's CAN IDs were reallocated 2026-05-19 from the original `0x20 / 0x30 / 0x31 / 0x32` block to the contiguous `0x36-0x39` block (Borealis claimed `0x20` for its safety frame). DBC and Plateau firmware were updated together — consumers integrating against the DBC can wire to the new IDs now.

- **Purpose**: Tilt/level measurement on both axes with per-corner height calculation
- **Hardware**: ESP32-S3-Zero with Adafruit BNO055 IMU
- **Framework**: ESP-IDF
- **Inputs**: BNO055 orientation data; CAN leveling configuration on `0x36`
- **Outputs**: CAN messages with tilt, per-corner heights, and status
- **CAN IDs**: 0x36 (LevelingConfig — RX), 0x37 (TiltData), 0x38 (CornerData), 0x39 (StatusData)
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

### TrailCurrent Solstice (MPPT Solar + SmartShunt Battery Gateway)
- **Purpose**: Bridge Victron MPPT (solar) and Victron SmartShunt (battery) onto CAN. Also handles MPPT load-output control
- **Hardware**: Waveshare ESP32-S3-RS485-CAN
- **Framework**: ESP-IDF
- **Inputs**: Victron MPPT via VE.Direct TEXT on UART1 (19200 baud, RX+TX); Victron SmartShunt via VE.Direct TEXT on UART2 (RX-only on current hardware)
- **Outputs**: SmartShunt basic battery data (voltage, current, SOC, wattage, time-to-go) on 0x23/0x24; MPPT solar data (panel V, wattage, battery V, charge state, panel current) on 0x2C/0x2D
- **CAN IDs**: 0x23 (ShuntBasicData1), 0x24 (ShuntBasicData2), 0x2C-0x2D (SolarMpptData1/2); accepts 0x2E (SolarLoadControl) and drives MPPT via VE.Direct HEX SET register 0xEDAB. Reserved: 0x2B (ShuntExtLive), 0x2F (ShuntExtHistory) — transmitted as zeros pending a TX wire to the SmartShunt for HEX GET
- **Documentation**: [Solstice.md](Solstice.md)
- **Source**: `/Product/TrailCurrentSolstice/`
- **Key Feature**: Single gateway for solar generation AND battery state-of-charge; supports MPPT load-output ON/OFF/Default control from the CAN bus

### TrailCurrent Switchback (Relay Module + Picket-style Sensor Input)
- **Purpose**: High-current relay switching for loads that don't fit the Torrent PWM profile. The same board doubles as a **Picket-compatible sensor input node** — its 8 digital inputs (the "8DI" in the board name) can be wired to reed switches, limit switches, current-loop opto inputs, or any dry-contact source, and it broadcasts their state in the same wire format Picket uses for door/cabinet sensors. The same physical Switchback can drive relays *and* report sensor inputs at the same time; there's no mode switch.
- **Hardware**: Waveshare ESP32-S3-ETH-8DI-8RO-C — 8 dry-contact relay outputs (8RO) plus 8 opto-isolated digital inputs (8DI) on the same board
- **Framework**: ESP-IDF
- **Inputs**: CAN toggle commands from UI/Headwaters (relay outputs); 8 opto-isolated digital input lines (sensor side — connect anything that closes a contact: doors, cabinets, bay lights, switch panels, current-loop signals)
- **Outputs**: Relay state bitmask on CAN; PicketStatus-format frames on CAN for the 8 input lines (consumed by exactly the same Headwaters code path that handles Picket modules — there's no Switchback-specific consumer logic)
- **Addressing**: Compile-time address (0-2) — up to 3 modules per bus via `build-all.sh`. Each Switchback instance owns one slot in *both* address pools simultaneously: the relay pool (`0x25-0x27` toggle, `0x28-0x2A` status) and the input pool (`0x12-0x14`, the extension of Picket's `0x0A-0x11` range).
- **CAN IDs**:
  - **Relay control** — `0x25-0x27` (toggle commands, RX), `0x28-0x2A` (status reports, TX)
  - **Sensor inputs** — `0x12-0x14` (PicketStatus8/9/10 wire format, TX). Address 0 = `0x12`, address 1 = `0x13`, address 2 = `0x14`. Same byte layout as PicketStatus0 (`0x0A`): byte 0 carries 8 inputs (1 bit per input, 1 = open / no current, 0 = closed), byte 1 is always `0x00` because Switchback has 8 inputs vs Picket's 12.
- **Documentation**: [Switchback.md](Switchback.md)
- **Source**: `/Product/TrailCurrentSwitchback/`
- **Key Feature**: Single board for both output (relay) and input (sensor) duty. In rigs where Picket coverage isn't worth a dedicated module — a few cabinet sensors here, a couple of limit switches there — a Switchback that's already driving relays can absorb the sensor load with no additional hardware. Headwaters, the PWA, and any other consumer treat the resulting frames as PicketStatus8/9/10, no Switchback-aware code required.

---

## Communication/Gateway Modules (Integration)

These modules provide connectivity and bridge external devices.

### TrailCurrent Aftline (Trailer Wiring Harness Monitor)
- **Purpose**: Monitor all 7 pins of trailer wiring (signals, lights, connection status)
- **Hardware**: Waveshare ESP32-S3-RS485-CAN
- **Framework**: ESP-IDF
- **Inputs**: ADC voltage sensing plus digital turn/brake/light lines from a 7-pin trailer connector
- **Outputs**: Trailer wiring status on CAN (lights, brakes, signals, connection)
- **CAN IDs**: 0x3A (TrailerStatus). Previously transmitted on 0x10, which collided with PicketStatus6 in Picket's reserved range; reallocated 2026-05-19.
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
- **Framework**: Custom Ubuntu Noble 24.04 image running a Python pipeline under systemd. Canonical install is the image build at [`TrailCurrentPeregrine/image_build/`](../../TrailCurrentPeregrine/image_build/); `deploy.sh` is a development tool only.
- **Inputs**: Microphone; system data via MQTT from Headwaters; any LAN browser via `https://peregrine.local/` (web chat UI); REST API surface exposed by `web_chat.py` for other clients on the rig (Headwaters, Playbill, Outbound, etc.) to invoke the on-device LLM
- **Outputs**: Voice responses (TTS), MQTT device commands, HTTP/WS responses to the LAN chat UI
- **Stack**: openWakeWord → faster-whisper (`base.en`, INT8 CPU) → **Llama 3.2 1B running on the Hexagon NPU via `genie-t2t-run`** (~12 tok/s) → Piper TTS (`en_US-libritts_r-medium`). All local, no cloud.
- **Wake Word**: Configurable (default "Hey Peregrine")
- **Intent set**: light/relay control, thermostat setpoint/threshold, vehicle leveling, GNSS/position queries, "what's playing on Playbill", radio tuning, plus the open-ended chat surface routed through the on-device LLM
- **Trust on Headwaters**: Peregrine's per-board self-signed CA is automatically installed in Headwaters' container trust store (via the Overlook PWA and the `peregrine-ca.js` service) so the in-vehicle dashboard and any backend HTTPS client can reach `https://peregrine.local/` without certificate warnings. The CA PEM is persisted to MongoDB (`system_config`) so a container recreate reinstalls it on startup.
- **Time source**: Headwaters (NTP, port 123) — see [NETWORK_TOPOLOGY.md — Time Synchronization](../01_Architecture/NETWORK_TOPOLOGY.md#time-synchronization)
- **Documentation**: [Peregrine.md](Peregrine.md)
- **Source**: `/Product/TrailCurrentPeregrine/` — see its `docs/` and `image_build/docs/` for the per-feature walkthroughs (build host setup, image build, flashing, first boot, first login, troubleshooting, development, cutting a release, software releases, MQTT CA cert, future vision modes)
- **Key Feature**: Fully offline pipeline — never leaves the vehicle (Core Principle: Data Privacy First). NPU-accelerated LLM is what makes "local-and-actually-useful" tractable on a passively-cooled rig device.

---

## Entertainment

### TrailCurrent Playbill (In-Rig Entertainment Head)
- **Purpose**: 10-foot, remote-driven entertainment head for use when the rig is parked — Live TV (OTA + RTL-SDR radio), local media library, streaming apps, screen mirroring / AirPlay. Not the vehicle dash head unit.
- **Hardware**: Radxa Dragon Q6A (Qualcomm QCS6490, 8 GB)
- **Framework**: Ubuntu Noble 24.04 + GNOME on Wayland, branded TrailCurrent throughout. The Playbill app is an Electron application (no TypeScript) installed on a normal desktop — not a kiosk lockdown.
- **Inputs**: Arrow keys / IR or Bluetooth remote, optional steering-wheel button MCU or other CAN-attached remote, Headwaters PWA (radio/volume/remote sub-pages), Peregrine voice intents ("what's playing on Playbill")
- **Outputs**: HDMI video, analog audio out (3.5 mm jack — pinned via WirePlumber so HDMI doesn't win); status frames on CAN; presence + state on MQTT
- **Addressing**: Up to **three Playbill instances per rig** (e.g. Living Room / Bedroom / Bunkhouse). Each instance claims one of three contiguous 16-ID CAN blocks: `0x100–0x10F`, `0x110–0x11F`, `0x120–0x12F`. Same message offset within each block. MQTT uses a human-readable `device.id` slug; CAN uses the numeric `device.canInstance`. A Playbill with `canInstance = null` is MQTT-only and consumes no CAN block.
- **CAN messages**: 10 message types per instance — `PlaybillNavCmd`, `PlaybillTransportCmd`, `PlaybillTransportStatus`, `PlaybillRadioTuneReq`, `PlaybillRadioStatus`, `PlaybillScreenStatus`, `PlaybillSystemCmd`, `PlaybillLaunchSourceCmd`, `PlaybillVolumeCmd`, `PlaybillPresence`. Full layouts in [CAN_BUS_REFERENCE.md](../10_Reference/CAN_BUS_REFERENCE.md#playbill-multi-instance-block).
- **MQTT topics**: `local/playbill/<deviceId>/<feature>/{command,status}`; broadcast variant `local/playbill/all/<feature>/command`; presence at `local/playbill/<deviceId>/system/status` (retained).
- **Time source**: Headwaters (NTP, port 123)
- **Documentation**: [Playbill.md](Playbill.md)
- **Source**: `/Product/TrailCurrentPlaybill/` — see its `docs/` for the per-feature documentation (Live TV, Radio, Cast, image build, embloader patch, Q6A lessons learned)
- **Key Feature**: Third-party CAN MCUs (steering-wheel buttons, IR receivers, hard-buttons remote panels) can drive a Playbill **directly on the CAN bus with no MQTT, no Headwaters service, and no cloud** — strongest demonstration to date that the platform is wire-only-capable end-to-end. See [Headwaters DOCS/CAN-REMOTE.md](../../TrailCurrentHeadwaters/DOCS/CAN-REMOTE.md) for the wire-level contract.

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
- Solstice (solar + battery monitoring — state-of-charge and consumption)
- Milepost (always-on display)

### Towing Setup
Add to above:
- Aftline (trailer wiring monitor)
- Spotter (in-vehicle trailer status display)

### Complete System
Add to above:
- Picket (cabinet & door sensors)
- Plateau (vehicle leveling)
- Peregrine (voice assistant)
- Fireside (portable wireless display)

### In-Rig Entertainment
Independent of the above — Playbill is a stationary-use product layered onto any rig that has Headwaters:
- Playbill (Live TV / radio / library / streaming / cast, up to 3 instances per rig)

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
┌────────┐ ┌───────────┐ ┌──────────────┐
│Therma  │ │Solstice   │ │Aftline       │
│Climate │ │Solar+Batt │ │Trailer       │
└────────┘ └───────────┘ └──────────────┘
     ▲       ▲          ▲
     │       │          │
     └───────┼──────────┘
             │
     Feedback Sensors:
     ├─ Borealis (environment)
     ├─ Solstice (solar + battery state-of-charge)
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

Entertainment:
└─ Playbill (in-rig entertainment head — up to 3 instances per rig,
             can also be driven directly from CAN by a button MCU)
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
