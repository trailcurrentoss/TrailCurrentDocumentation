# TrailCurrent Documentation Status

## Overview

This document tracks the status of TrailCurrent documentation creation and completion.

**Last Updated**: 2026-05-19
**Status**: Core structure complete. Playbill onboarded (Tier 1 sweep, see entry below). CAN Bus Reference refreshed from DBC. Hardware module docs reflect the full ESP-IDF migration (all modules except Spotter) plus the Linux-class devices (Peregrine, Playbill) that participate on the CAN bus. Cloud Application README matches Farwatch (MongoDB, API key auth, PWA). Mobile Application README covers both Outbound (Android native) and the React Native app. Vehicle Compute README points at CM5 + carrier board hardware. BUILD_SYSTEM.md ESP-IDF section filled in.

> Explicit module counts are no longer tracked in this index — the platform's product mix is growing across multiple form factors (ESP32 modules, Pi CM5 compute, Radxa Q6A Linux-class devices) and a single number is more misleading than useful. See [02_Hardware_Modules/MODULE_CATEGORIES.md](02_Hardware_Modules/MODULE_CATEGORIES.md) for the current authoritative list.

### Switchback dual-capability disclosure (2026-05-19, Tier 2.8)

Switchback's 8 digital inputs (the "8DI" in the Waveshare ESP32-S3-ETH-8DI-8RO-C board name) had been emitting on `0x12-0x14` in PicketStatus wire format for some time, but the central docs only described Switchback as a relay module. This sweep brought the central docs in line with what the firmware (and the DBC) already say.

- `02_Hardware_Modules/MODULE_CATEGORIES.md` — Switchback section retitled "Relay Module + Picket-style Sensor Input"; entry rewritten to describe the same physical board doing relay output and sensor input simultaneously, two-pool addressing (each instance owns one slot in *both* the relay pool `0x25-0x2A` and the input pool `0x12-0x14`), explicit byte-layout note (byte 0 = 8 inputs, byte 1 = 0x00 because Switchback has 8 inputs vs Picket's 12), Key Feature describing the consolidation pattern for rigs that don't need a dedicated Picket.
- `02_Hardware_Modules/README.md` — one-liner updated from "6-channel relay" (stale, hardware is 8-channel) to "8-channel dry-contact relay … plus 8 opto-isolated digital inputs that double as a Picket-style sensor node"; CAN IDs by Module block now shows the `0x12-0x14` extension with a note tying it to the Picket address pool; Links to Source Code line updated.
- `10_Reference/CAN_BUS_REFERENCE.md` — Bus Nodes row for Switchback rewritten with the dual-capability description; Switchback detail-section header changed to "(0x12-0x14, 0x25-0x2A)"; intro paragraph rewritten with a two-pool addressing table linking address 0/1/2 to `(0x25, 0x28, 0x12) / (0x26, 0x29, 0x13) / (0x27, 0x2A, 0x14)`. Picket section gained a forward reference to the Switchback section explaining the address-pool extension. No DBC changes needed (DBC already had Switchback as the transmitter on `BO_ 18/19/20`).

No firmware changes — Switchback's source already emits these frames.

### CAN ID closeout — WifiConfigProvisioning added, Plateau block reallocated (2026-05-19, Tier 2.7)

After the Tier 2.5 collision sweep, two follow-up items remained: `0x01 WifiConfigProvisioning` had to be added to the DBC (it was used by every ESP-IDF module's firmware but absent from the DBC), and Plateau's now-orphaned ID block (`LevelingConfig` displaced from `0x20`, plus `0x30/0x31/0x32` for TiltData/CornerData/StatusData) had to be reallocated together. Closed both in one pass so other agents working on the platform see the final allocation and don't reintroduce drift.

**WifiConfigProvisioning** — `BO_ 1` added to the DBC (DLC 8, Headwaters transmitter, 12 module receivers). Empirically validated with `cantools` to confirm no parser issues. A `CM_ BO_ 1` comment captures the full multi-frame protocol (Subcommand byte 0: 0x01=Start, 0x02=SSID-chunk, 0x03=Password-chunk, 0x04=End-with-XOR-checksum; 6-byte chunks; 50 ms inter-frame). New "WiFi Credential Provisioning (0x01)" section in `10_Reference/CAN_BUS_REFERENCE.md` with the full table. No firmware changes — every firmware was already on `0x01`; the DBC was the laggard.

**Plateau reallocation** — entire block moved to contiguous `0x36-0x39` (Plateau's first available slot after the existing MCU range, with `0x36-0x39` reserved together for the leveling group):

| Frame | Old ID | New ID | DBC entry |
|---|---|---|---|
| LevelingConfig | `0x20` (displaced by Borealis) | `0x36` | `BO_ 54` (new) |
| TiltData       | `0x30` | `0x37` | `BO_ 55` (was `BO_ 48`) |
| CornerData     | `0x31` | `0x38` | `BO_ 56` (was `BO_ 49`) |
| StatusData     | `0x32` | `0x39` | `BO_ 57` (was `BO_ 50`) |

DBC: BO_ entries renumbered, signal definitions migrated, `CM_ BO_` and `CM_ SG_` comments renumbered (the stale `CM_ SG_ 32 *` comments that were orphaned when Borealis took 0x20 are now correctly `CM_ SG_ 54 *`). `CM_ BU_ Plateau` description rewritten with new IDs and a note about the reallocation. `cantools` parse confirms 88 messages, no errors; `0x30/0x31/0x32` correctly show as freed.

Plateau firmware: `main/main.c` and `main/discovery.c` `#define`s updated to the new IDs with explanatory comments. Plateau's `README.md` CAN ID list updated.

Central docs: `CAN_BUS_REFERENCE.md` main allocation table updated (old Plateau rows removed, new `0x36-0x39` rows added, `0x01` row clarified as multi-frame). Plateau detail section header changed from "(0x30-0x32 — pending re-allocation)" to "(0x36-0x39)" with the warning callout replaced by a one-line note that consumers can wire to the new IDs now. Per-message subsection headers updated (`TiltData (0x37, ...)` etc). Message-frequency table rows updated. `02_Hardware_Modules/README.md` "CAN IDs by Module" block updated (old Plateau lines deleted, new `0x36-0x39` block added). `02_Hardware_Modules/MODULE_CATEGORIES.md` Plateau entry warning changed from "pending re-allocation" to "firmware not yet shipping" with the new IDs.

**Net effect**: the DBC + every firmware that has CAN_ID `#define`s for the affected messages are now in sync. Plateau, when it ships, will have a clean contiguous block. `0x30 / 0x31 / 0x32` are now genuinely free for future use.

### CAN ID collision sweep — Borealis safety frame, Aftline relocation, Borealis sensor refresh (2026-05-19, Tier 2.5)

A cross-repo firmware-vs-DBC audit (every module's `main/main.c` + Headwaters' Node.js CAN bridge, against the live DBC) surfaced two CAN ID collisions that needed coordinated firmware + DBC + central-doc fixes. Both resolved in one sweep.

**Conflict 1 — `0x20`**: Borealis firmware uses `0x20` for a new combined safety frame (CO, LPG, alarm flags); DBC had `0x20 = LevelingConfig` (Headwaters → Plateau). Plateau firmware exists but isn't shipping yet — IDs are eligible to move. Decision: keep Borealis on `0x20` (already deployed), displace `LevelingConfig`.

**Conflict 2 — `0x10`**: Aftline firmware uses `0x10` for `TrailerStatus`; DBC reserves `0x0A-0x14` as `PicketStatus0..10` (Picket's per-instance status pool). Picket's `main.c:54-56` explicitly comments that this range is for door-sensor frames in a shared wire format. Aftline at `0x10` (= `PicketStatus6`) was squatting on a slot with structurally different payload semantics. Decision: move Aftline `TrailerStatus` to `0x3A` (free slot in the dense MCU range, between TorrentSequence and Reservoir).

Also caught and corrected in the same sweep: Borealis's new sensor generation (SCD41 + SGP40 + SEN0466 CO + SEN0131 LPG, on a Waveshare ESP32-S3-RS485-CAN board) changed the *semantics* of `EnvironmentSensorData` bytes 4-7 — previously TVOC ppb + eCO2 ppm from an SGP30, now **real CO2 ppm** from the SCD41 and **VOC Index 1-500** from the SGP40. Byte positions unchanged; signals on those bytes changed. Any consumer that decoded the old layout will return nonsense.

Audit also confirmed `0x12-0x14` (Switchback `CAN_ID_INPUT_BASE`) is **not** a conflict — Switchback intentionally transmits PicketStatus8/9/10-compatible frames from its digital inputs (Picket's source code endorses the shared wire format). That's a documented multi-transmitter pattern, not a collision.

**Files changed (this sweep):**

- `TrailCurrent.dbc` — `BO_ 32` retitled `LevelingConfig` → `BorealisSafetyData` (3 signals: CarbonMonoxidePpm, LpgRatioScaled, AlarmFlags). New `BO_ 58 TrailerStatus` (6 signals: Connected, LeftTurn, RightTurn, RunningLights, Brakes, TrailerVoltageMv) added between TorrentSequence2 and WaterTankLevels. `BO_ 31 EnvironmentSensorData` signal names updated (`TVOC` → `CO2Ppm`, `eCO2` → `VOCIndex` with new range 1-500). `CM_ BU_ Borealis` rewritten to describe new hardware + sensors. `CM_ BO_ 31` rewritten with explicit byte-4-7-semantics-changed warning. `CM_ BO_ 32` rewritten for safety frame. New `CM_ BO_ 58` added.
- `10_Reference/CAN_BUS_REFERENCE.md` — main allocation table updated for `0x1F`/`0x20`/`0x3A`. Plateau IDs flagged "pending re-allocation". Bus Nodes row for Borealis rewritten for new hardware. Borealis detail section rebuilt (`EnvironmentSensorData` byte semantics, new `BorealisSafetyData` table with thresholds, "wire-format change" warning callout). New `Aftline - Trailer 7-Pin Connector Monitor (0x3A)` detail section. Plateau detail section prefixed with a pending-re-allocation admonition. Overview's "next available" pointer updated for the new gaps.
- `02_Hardware_Modules/README.md` — Borealis moved from "ESP32-S3-Zero" to "Waveshare ESP32-S3-RS485-CAN" in the hardware variant table; CAN IDs by Module block updated for `0x20` (now BorealisSafetyData), `0x3A` (Aftline TrailerStatus), and Plateau pending re-allocation.
- `02_Hardware_Modules/MODULE_CATEGORIES.md` — Borealis entry retitled "Environment & Safety", board change, expanded sensor list, both CAN IDs, on-board alarm thresholds. Aftline entry CAN ID updated to `0x3A` with a note explaining the move. Plateau entry prefixed with a "firmware not yet shipping; ID block pending re-allocation" warning.
- Aftline firmware: `main/main.c:27-29` and `main/discovery.c:24-26` — `CAN_ID_TRAILER_STATUS` / `CAN_STATUS_ID` moved from `0x10` to `0x3A`, with explanatory comments. Aftline `README.md` — TX table and mDNS canid TXT-record reference updated to `0x3A`.

**Closed in subsequent Tier 2.7 sweep** (see entry above):

- ✅ `0x01 WifiConfigProvisioning` — `BO_ 1` added to DBC; cantools-validated.
- ✅ Plateau's full ID block reallocated to `0x36-0x39` (LevelingConfig / TiltData / CornerData / StatusData) in DBC + firmware in one coordinated edit.

**Verified non-issue**: `0x12-0x14` PicketStatus8/9/10 — re-reading the live DBC, these already have `Switchback` as the transmitter field on `BO_ 18/19/20`, and the `CM_ BO_` comments (both the Picket umbrella `CM_ BO_ 10` and the per-message `CM_ BO_ 18/19/20`) already describe Switchback as the sender and explain the shared wire format with Picket. The message *name* (`PicketStatusN`) is intentionally inherited from the wire-format-compatible Picket frame, but the transmitter field is correctly `Switchback`. Confirmed via `cantools` — `senders=['Switchback']` on all three IDs. No edit needed.

### Time synchronization, Headwaters Q6A variant, wireless discovery, Peregrine refresh (2026-05-19, Tier 2)

Tier 2 brought four threads into the central docs in one pass — all reflecting work that was already real in the source repos but unrepresented in the index.

**Time synchronization (foundational, gets a dedicated callout)** — Headwaters is the canonical LAN NTP server for the rig. Every Linux-class device on the platform (Peregrine, Playbill, future compute modules) syncs to it via `chronyd` on UDP 123, with `time-from-bearing.service` feeding GNSS UTC into the system clock when no upstream NTP pool is reachable. This is what makes offline scheduling possible — every device evaluates triggers against the same clock with or without internet.

- `01_Architecture/NETWORK_TOPOLOGY.md` — new **Time Synchronization** section with the architectural rationale, the three-tier chronyd source resolution (pool → GNSS → local stratum 10), an ASCII diagram of the topology, and configuration pointers. Firewall block updated to allow `123/UDP` from the rig LAN and `8883/MQTTS` from in-vehicle clients (the prior rule incorrectly implied MQTT was localhost-only).
- `03_Vehicle_Compute/README.md` — new **Time Synchronization** subsection with the same rationale framed for the Headwaters operator audience; NTP added as Key Component #6; intro bullet list now calls out NTP as a Headwaters responsibility.
- `01_Architecture/PLATFORM_OVERVIEW.md` — Edge Layer Components list now explicitly enumerates NTP serving as a Headwaters responsibility, with a link to the topology doc.
- `10_Reference/README.md` — **Port Numbers** table now includes `NTP / 123 / UDP` and the Peregrine LAN web chat at `443/HTTPS`. The fictional "Message IDs (CAN)" allocation table (`GPS 0x100`, `Power 0x200`, etc. — none of which match the real DBC) was replaced with a two-row at-a-glance summary that points at `CAN_BUS_REFERENCE.md` as the authoritative source.

**Headwaters Radxa Q6A variant** — work in progress alongside the canonical CM5 build, with full self-contained image build + flash pipeline under `RADXAQ6A/`. Captured but explicitly flagged as not-recommended-for-shipping.

- `03_Vehicle_Compute/README.md` — new banner-style admonition introducing the Q6A variant as an alternate hardware platform under active development. CM5 remains canonical.
- `01_Architecture/PLATFORM_OVERVIEW.md` — Edge Layer Compute Device entry now mentions the Q6A variant with the same WIP framing.

**Wireless module discovery + OTA** — Fireside (battery-powered wireless touchscreen, no CAN attachment) forced the split between the CAN-broadcast OTA path and a parallel mDNS + MQTT discovery path. The pattern is intentionally generic.

- `08_Deployment/FIRMWARE_UPDATES.md` — new top-of-file **Overview** table contrasting the three device classes (CAN-attached MCU, wireless module, Linux-class compute), followed by a **Wireless Module Discovery** section that documents the `discovery/browse/*`, `discovery/confirm/*`, and `discovery/claim/*` MQTT topic family and the two onboarding patterns (`confirm` for MCUs, `claim` for Linux-class). A short **Deployment Package Distribution (Zip via PWA)** section follows describing the offline / local-loop side-load path through Overlook.

**Peregrine refresh** — central-doc entry was technically correct but understated several Stage-2 advancements. Folded the Tier 3 plan into this Tier 2 pass since the work was small.

- `02_Hardware_Modules/MODULE_CATEGORIES.md` — Peregrine entry rewritten: LLM runtime corrected to `genie-t2t-run` on Hexagon NPU (~12 tok/s); LAN web chat UI at `https://peregrine.local/` and the REST API surface it exposes for other clients on the rig are now first-class; intent set enumerated (lights/relays, thermostat, leveling, GNSS, "what's playing on Playbill", radio tuning, open-ended chat); CA-cert auto-install in Headwaters' trust store via `peregrine-ca.js` documented; Headwaters NTP listed as the explicit time source; canonical install path identified as the image build (`deploy.sh` clarified as a dev tool).
- `01_Architecture/PLATFORM_OVERVIEW.md` — Peregrine one-liner in Hardware Module Categories now names the NPU runtime and the LAN web chat URL.

Out of scope for this sweep (deferred / not yet needed):
- Borealis sensor expansion (commit `15bf6e8` moved to ESP32-S3 with new sensors — verify the final sensor list with Borealis source before updating central docs)
- Switchback Picket-bridge feature (commit `44d1715` — small, will fold into the next sweep)
- `TrailCurrentCANLibrary` shared-library callout in `06_Shared_Libraries/README.md` — most modules now consume it; worth a dedicated paragraph but not urgent

### Playbill onboarding (2026-05-19, Tier 1)

Playbill (the in-rig entertainment head, Radxa Dragon Q6A running Ubuntu Noble 24.04 + GNOME + an Electron app) had landed in the source repos and the live DBC (`BU_ Playbill`, `BO_ 256–303`) but was entirely absent from the central docs. Tier 1 brought the central index into alignment without claiming scope beyond what's real today:

- `02_Hardware_Modules/MODULE_CATEGORIES.md` — new top-level **Entertainment** section (sibling to "Voice & AI Modules") with a Playbill entry: hardware, app shell, audio routing, multi-instance addressing (`canInstance` 0/1/2 → `0x100`/`0x110`/`0x120` blocks), MQTT topic root, time source, and the wire-only-from-CAN key feature. Added a new "In-Rig Entertainment" use-case row (deliberately independent of the other use cases — Playbill is a stationary-use add-on, not a stack on top of basic monitoring).
- `02_Hardware_Modules/Playbill.md` — new stub. Keeps central content summary-level and defers everything operational (build, flash, per-feature docs, Q6A lessons learned) to `/Product/TrailCurrentPlaybill/docs/`.
- `02_Hardware_Modules/README.md` — opener broadened from "ESP32-based" to "CAN-attached hardware" so it correctly covers the Linux-class devices on the bus. Dropped the "Module Statistics" table (Total = 16) — explicit count was misleading given Playbill, the multi-instance pattern, and the Q6A variant work. Added Entertainment as category #6. Extended the "CAN IDs by Module" block with the three Playbill instance blocks (`0x100–0x12F`). Added `TrailCurrentPlaybill/` to the Links to Source Code list.
- `10_Reference/CAN_BUS_REFERENCE.md` — Overview now describes two ranges (hardware-module `0x00–0x42`, Playbill head-unit `0x100–0x12F`). New summary row in the ID allocation table linking to the detail section. Added Playbill to the Bus Nodes table. New **Playbill multi-instance block** detail section with the full 10-message-per-instance map, all signals/enums/bit positions, cross-cutting design notes, and pointers to Headwaters `DOCS/CAN-REMOTE.md` (the wire-level contract) and Playbill's `docs/app/dbc-additions.md` (the narrative).
- `01_Architecture/PLATFORM_OVERVIEW.md` — Device Layer now lists Reservoir and Switchback (previously missing too), plus Entertainment (Playbill). Technology line updated to acknowledge ESP32 + Radxa Q6A coexisting on the bus. New "Entertainment" subsection in the Hardware Module Categories block.
- `01_Architecture/DATA_FLOW.md` — new **Scenario 5: CAN-Only Remote Control of Playbill (No MQTT, No Headwaters Service)**, demonstrating the wire-only-from-CAN path end-to-end. Original Scenario 5 (Offline Operation) renumbered to Scenario 6.
- `10_Reference/MQTT_TOPICS.md` — new **Playbill Topics (local/playbill/*)** subsection covering per-instance command/status topics, the `all` broadcast variant, the retained `now-playing` payload (MQTT-only — does not live on CAN), and the Headwaters REST/WS surface that fans them out.

Out of scope for this sweep (planned for Tier 2 / Tier 3):
- Headwaters NTP / time-from-bearing architectural callout (foundational for scheduling — every Linux-class compute device on the rig syncs to Headwaters)
- Headwaters Radxa Q6A variant (work-in-progress)
- Wireless module discovery + OTA (Fireside is the first; pattern is generic)
- Peregrine LLM runtime correction (`genie-t2t-run` on Hexagon NPU, not ollama) + LAN web chat UI + expanded intent list

### Ampline consolidation (2026-04-18)

Ampline (formerly a standalone Victron SmartShunt bridge) has been absorbed into Solstice. Solstice now drives both the MPPT (UART1 bidirectional, VE.Direct TEXT + HEX SET for load control) and the SmartShunt (UART2 RX-only for VE.Direct TEXT). Documentation and the DBC were swept to match:

- DBC: `Ampline` removed from `BU_:` node list; `BO_ 35` and `BO_ 36` transmitter changed to `Solstice` and renamed from `BatteryShuntData1/2` to `ShuntBasicData1/2` to match Solstice firmware. `CM_ BU_ Ampline` deleted; `CM_ BU_ Solstice` rewritten to describe the unified role. `CM_ BO_ 43` / `CM_ BO_ 47` flagged as reserved — Solstice transmits these frames with all-zero payload because the SmartShunt UART is RX-only on the current hardware (HEX GET needs a TX wire).
- Hardware module docs, architecture docs, reference docs, cloud README, marketing draft, and the discovery-NVS working prompt all had `Ampline` references removed or re-pointed at Solstice.
- `DATA_FLOW.md` had a secondary drift fixed in the same sweep: "Ampline PDM" was incorrectly labelling the power-distribution-module role, which actually belongs to Torrent. Those references now correctly say `Torrent`.
- Module count dropped from 17 to 16.

### Recent sweep (2026-04-11 — second pass)

Second pass across the remaining areas:

- `01_Architecture/DEPLOYMENT_ARCHITECTURE.md` — cloud tier rewritten around Farwatch (single-VPS default; MongoDB instead of PostgreSQL+Redis; Let's Encrypt; proximity automation engine). Scaling guidance is now "vertical by default, horizontal only if measured"
- `01_Architecture/PLATFORM_OVERVIEW.md` — cloud components updated to Farwatch (MongoDB + tileserver + PWA); hardware module list refreshed to include Reservoir and Switchback; Key Technologies table refreshed with ESP-IDF v5.1+ and MongoDB
- `01_Architecture/SYSTEM_ARCHITECTURE.md` — cloud tier diagram updated (Farwatch: Backend, MongoDB, Mosquitto TLS, tileserver, package storage); scalability section rewritten around MongoDB replica sets and backend replicas
- `01_Architecture/DATA_FLOW.md` — added Scenario 3 (Reservoir water tanks) and Scenario 4 (Proximity automation, phone ↔ vehicle via Farwatch); Scenario 3 "Offline Operation" renumbered to Scenario 5
- `06_Shared_Libraries/README.md` — reflects ESP-IDF migration; C6-specific libraries documented as retired (modules moved to Waveshare S3 boards); remaining WROOM32 libraries scoped to Torrent/Tapper
- `08_Deployment/README.md` — "Infrastructure Scaling" rewritten around vertical scaling (default) + MongoDB replica set + backend replicas
- `10_Reference/GLOSSARY.md` — PostgreSQL/Redis/JWT entries replaced with MongoDB and API Key; CAN Transceiver entry corrected (SN65HVD230 on modules, MCP2515 HAT on Headwaters)
- `10_Reference/README.md` — network ports table updated (MongoDB 27017, tileserver 8080, MQTT TLS 8883 annotation for vehicle↔cloud)
- `10_Reference/HARDWARE_SPECIFICATIONS.md` — CAN Transceiver section filled in: SN65HVD230 on modules, MCP2515 on the Waveshare RS485 CAN HAT (B) for Headwaters
- `10_Reference/EXTERNAL_LINKS.md` — Database Tools section populated with MongoDB references (Manual, mongosh, Compass, backup docs)

### Recent sweep (2026-04-11 — first pass)

Documentation drift was audited against the current state of every project under
`/Product/` and the following files were brought up to date:

- `02_Hardware_Modules/README.md` — framework claims corrected (all ESP-IDF except Spotter); Reservoir added; CAN ID map expanded; hardware variant table rewritten; source paths fixed (Bearing was referencing the old `TrailCurrentGnssModule/` path)
- `02_Hardware_Modules/MODULE_CATEGORIES.md` — all CAN ID ranges corrected to the real DBC values; Reservoir + Switchback added as full entries; Peregrine updated to reflect the Radxa Dragon Q6A + offline pipeline
- `02_Hardware_Modules/Firmware/PlatformIO_Setup.md` — marked legacy / Spotter-only
- `10_Reference/CAN_BUS_REFERENCE.md` — message table updated (added BorealisCalibration, ShuntExt, SolarLoadControl, TorrentSequence, WaterTankLevels, per-address Torrent and Switchback rows); Bus Nodes table now includes Reservoir and fixes Headwaters, Aftline, Switchback, Borealis hardware
- `03_Vehicle_Compute/README.md` — overview rewritten to explicitly reference CM5 + Waveshare RS485 CAN HAT (B) and to point at Baseflow as the lite variant
- `04_Cloud_Application/README.md` — fully rewritten to match Farwatch reality (MongoDB not PostgreSQL, API key auth not JWT, PWA + WebSocket architecture, proximity automation, deployment packages, data minimization stance)
- `05_Mobile_Application/README.md` — fully rewritten to cover both TrailCurrent Outbound (Android native, Kotlin + Jetpack Compose + MapLibre) and the React Native / Expo app
- `07_Development/BUILD_SYSTEM.md` — ESP-IDF section and backend build section filled in (replacing NEEDS TO BE COMPLETED stubs)

---

## Documentation Categories

### ✅ Root Documentation (5/5 Complete)

Core project documentation providing overview and guidance:

- [x] **README.md** - Main entry point with platform overview
- [x] **CORE_PRINCIPLES.md** - Foundational principles (Cloud Optional, Data Private, 100% Open Source, Safety First)
- [x] **QUICK_START.md** - 5-minute getting started guide
- [x] **WHAT_IS_SOFTWARE_DEFINED_VEHICLE.md** - Why TrailCurrent differs from generic IoT
- [x] **LIABILITY_AND_DISCLAIMER.md** - Legal notice and disclaimer

### ✅ Architecture Documentation (5/5 Complete)

System design and architecture:

- [x] **01_Architecture/PLATFORM_OVERVIEW.md** - System layers and architecture
- [x] **01_Architecture/SYSTEM_ARCHITECTURE.md** - Detailed system design
- [x] **01_Architecture/NETWORK_TOPOLOGY.md** - Network layout and connectivity
- [x] **01_Architecture/DATA_FLOW.md** - Information flow through system
- [x] **01_Architecture/DEPLOYMENT_ARCHITECTURE.md** - Infrastructure design

### ✅ Hardware Module Documentation (4/4 Complete)

Hardware module documentation:

- [x] **02_Hardware_Modules/README.md** - Hardware overview and module list (covers all CAN-attached devices — ESP32 modules and Linux-class compute)
- [x] **02_Hardware_Modules/MODULE_CATEGORIES.md** - Module classification and organization
- [x] **02_Hardware_Modules/Firmware/ESP_IDF_Setup.md** - ESP-IDF firmware setup (primary setup for all modules)
- [x] **02_Hardware_Modules/Firmware/PlatformIO_Setup.md** - PlatformIO setup (legacy; Spotter-only)

**📋 Per-Module Detail Files**:
- [x] Playbill (in-rig entertainment head) — summary stub at [02_Hardware_Modules/Playbill.md](02_Hardware_Modules/Playbill.md); operational detail in `/Product/TrailCurrentPlaybill/docs/`
- [ ] Bearing (GNSS) documentation
- [ ] Borealis (environment) documentation
- [ ] Torrent (power delivery) documentation
- [ ] Therma (closed-loop thermostat) documentation
- [ ] Solstice (solar + battery gateway) documentation
- [ ] Plateau (vehicle level) documentation
- [ ] Picket (cabinet & door sensors) documentation
- [ ] Switchback (relay module) documentation
- [ ] Aftline (trailer wiring) documentation
- [ ] Reservoir (water tank levels) documentation
- [ ] Tapper (8-button panel) documentation
- [ ] Fireside (wireless display) documentation
- [ ] Milepost (hardwired display) documentation
- [ ] Spotter (trailer monitor display) documentation
- [ ] Peregrine (voice assistant) documentation

### ✅ In-Vehicle Compute Documentation (3/3 Complete)

Edge computing system documentation:

- [x] **03_Vehicle_Compute/README.md** - Edge compute overview (emphasizes local operation & optional cloud)
- [x] **03_Vehicle_Compute/SETUP_GUIDE.md** - Step-by-step setup guide
- [x] **03_Vehicle_Compute/Deployment/PI_DEPLOYMENT.md** - Deployment procedures

### ✅ Cloud Application Documentation (2/2 Complete)

Cloud services documentation:

- [x] **04_Cloud_Application/README.md** - Cloud application overview
- [x] **04_Cloud_Application/Deployment/CLOUD_DEPLOYMENT.md** - Cloud deployment guide

### ✅ Mobile Application Documentation (1/1 Complete)

Android application documentation:

- [x] **05_Mobile_Application/README.md** - Mobile app overview

### ✅ Shared Libraries Documentation (1/1 Complete)

Common libraries and utilities:

- [x] **06_Shared_Libraries/README.md** - Shared libraries overview

### ✅ Development Documentation (5/5 Complete)

Developer guidelines and procedures:

- [x] **07_Development/README.md** - Development overview
- [x] **07_Development/CONTRIBUTING.md** - Contribution guidelines (with Core Principles enforcement)
- [x] **07_Development/DEVELOPMENT_SETUP.md** - Development environment setup
- [x] **07_Development/BUILD_SYSTEM.md** - Build procedures and optimization
- [x] **07_Development/TESTING_GUIDE.md** - Testing frameworks and procedures
- [x] **07_Development/DEBUGGING.md** - Debugging techniques and tools

### ✅ Deployment Documentation (5/5 Complete)

Deployment and operation procedures:

- [x] **08_Deployment/README.md** - Deployment overview
- [x] **08_Deployment/DEPLOYMENT_CHECKLIST.md** - Pre-deployment verification
- [x] **08_Deployment/DEPLOYMENT_SCRIPTS.md** - Automation scripts reference
- [x] **08_Deployment/FIRMWARE_UPDATES.md** - OTA update procedures
- [x] **08_Deployment/DOCKER_SETUP.md** - Docker configuration guide

### ✅ Troubleshooting Documentation (5/5 Complete)

Diagnostic and problem-solving guides:

- [x] **09_Troubleshooting/README.md** - Troubleshooting overview
- [x] **09_Troubleshooting/COMMON_ISSUES.md** - Common problems and solutions
- [x] **09_Troubleshooting/HARDWARE_ISSUES.md** - Hardware troubleshooting guide
- [x] **09_Troubleshooting/NETWORK_ISSUES.md** - Network troubleshooting guide
- [x] **09_Troubleshooting/FIRMWARE_ISSUES.md** - Firmware troubleshooting guide
- [x] **09_Troubleshooting/LOGGING_AND_DIAGNOSTICS.md** - Diagnostic procedures

### ✅ Reference Documentation (7/7 Complete)

Technical reference materials:

- [x] **10_Reference/README.md** - Reference overview
- [x] **10_Reference/GLOSSARY.md** - Technical terminology and acronyms
- [x] **10_Reference/CAN_BUS_REFERENCE.md** - CAN message formats and IDs (fully populated from DBC)
- [x] **10_Reference/MQTT_TOPICS.md** - MQTT topic hierarchy and messages
- [x] **10_Reference/GPIO_PIN_MAPPING.md** - Pin assignments and wiring
- [x] **10_Reference/HARDWARE_SPECIFICATIONS.md** - Electrical and physical specs
- [x] **10_Reference/EXTERNAL_LINKS.md** - External resources and documentation
- [x] **TrailCurrent.dbc** - Machine-readable CAN database file (authoritative source for all CAN message definitions)

---

## Documentation Structure Summary

```
TrailCurrent Documentation (43 total files)
├── Root Documents (5)
│   ├── README.md
│   ├── CORE_PRINCIPLES.md ⭐ CRITICAL
│   ├── QUICK_START.md
│   ├── WHAT_IS_SOFTWARE_DEFINED_VEHICLE.md
│   └── LIABILITY_AND_DISCLAIMER.md
├── 01_Architecture (5)
│   ├── PLATFORM_OVERVIEW.md
│   ├── SYSTEM_ARCHITECTURE.md
│   ├── NETWORK_TOPOLOGY.md
│   ├── DATA_FLOW.md
│   └── DEPLOYMENT_ARCHITECTURE.md
├── 02_Hardware_Modules (4 + 15 placeholders)
│   ├── README.md
│   ├── MODULE_CATEGORIES.md
│   ├── Firmware/ESP_IDF_Setup.md
│   └── Firmware/PlatformIO_Setup.md
├── 03_Vehicle_Compute (3)
│   ├── README.md
│   ├── SETUP_GUIDE.md
│   └── Deployment/PI_DEPLOYMENT.md
├── 04_Cloud_Application (2)
│   ├── README.md
│   └── Deployment/CLOUD_DEPLOYMENT.md
├── 05_Mobile_Application (1)
│   └── README.md
├── 06_Shared_Libraries (1)
│   └── README.md
├── 07_Development (6) ⭐ CONTRIBUTING.md is CRITICAL
│   ├── README.md
│   ├── CONTRIBUTING.md ⭐ ENFORCES CORE PRINCIPLES
│   ├── DEVELOPMENT_SETUP.md
│   ├── BUILD_SYSTEM.md
│   ├── TESTING_GUIDE.md
│   └── DEBUGGING.md
├── 08_Deployment (5)
│   ├── README.md
│   ├── DEPLOYMENT_CHECKLIST.md
│   ├── DEPLOYMENT_SCRIPTS.md
│   ├── FIRMWARE_UPDATES.md
│   └── DOCKER_SETUP.md
├── 09_Troubleshooting (6)
│   ├── README.md
│   ├── COMMON_ISSUES.md
│   ├── HARDWARE_ISSUES.md
│   ├── NETWORK_ISSUES.md
│   ├── FIRMWARE_ISSUES.md
│   └── LOGGING_AND_DIAGNOSTICS.md
├── 10_Reference (7)
│   ├── README.md
│   ├── GLOSSARY.md
│   ├── CAN_BUS_REFERENCE.md ⭐ FULLY POPULATED
│   ├── MQTT_TOPICS.md
│   ├── GPIO_PIN_MAPPING.md
│   ├── HARDWARE_SPECIFICATIONS.md
│   └── EXTERNAL_LINKS.md
└── TrailCurrent.dbc ⭐ AUTHORITATIVE CAN DATABASE
```

---

## Key Principles Embedded

### ⭐ Core Principles (All 4 Enforced)

1. **☁️ Cloud Optional** - Documented in every architecture and deployment guide
2. **🔐 Data Privacy First** - Emphasized in CONTRIBUTING.md and deployment guides
3. **📖 100% Open Source** - Required in CONTRIBUTING.md and design guidelines
4. **⚠️ Safety First** - Physical wired control required for all critical devices

### 🎯 Critical Documents

- **CORE_PRINCIPLES.md** - Non-negotiable principles guard against architectural violations
- **CONTRIBUTING.md** - Enforces principles in code reviews with explicit checklists
- **PLATFORM_OVERVIEW.md** - Makes Cloud layer optional and private
- All deployment docs emphasize local operation first, cloud sync secondary

---

## Documentation Completion Status

### Fully Completed (with content)
✅ **23 Files** - These files contain full structure, content outlines, and detailed organization:
- Root documents (3)
- Architecture (5)
- Development (6)
- Troubleshooting (6)
- Deployment (5)
- Reference (7)
- Component overviews (3)

### Placeholder/Stub Structure (ready for content)
📋 **20 Files** - These files have complete structure and "NEEDS TO BE COMPLETED" sections indicating what should go in each:
- Hardware module specifics (14)
- DEVELOPMENT_SETUP.md subsections
- Various deployment/troubleshooting subsections
- Reference file subsections

---

## What Each File Type Contains

### Reference Files
- Purpose statement
- Overview/Introduction (placeholder for completion)
- Section headings for expected content
- "NEEDS TO BE COMPLETED" blocks indicating what information belongs where
- Related documentation links
- Cross-references to other docs

### Development Guides
- Full structure with detailed headings
- Prerequisites and setup steps
- Step-by-step procedures
- Troubleshooting sections
- Related documentation links
- Best practices and guidelines

### Troubleshooting Guides
- Symptoms identification
- Diagnosis procedures
- Solution steps
- Tool references
- Related documentation
- Examples of correct vs incorrect

### Deployment Checklists
- Complete verification lists
- Go/no-go decision points
- Pre, during, and post-deployment phases
- Safety procedures
- Rollback procedures

---

## Next Steps for Documentation Completion

### Priority 1 (Referenced 5+ times)
Complete these to resolve most broken links:
- [x] CAN_BUS_REFERENCE.md - message formats ✅ Fully populated from DBC (2026-03-14)
- [ ] MQTT_TOPICS.md - topic definitions
- [ ] GPIO_PIN_MAPPING.md - complete pin assignments per module
- [ ] HARDWARE_SPECIFICATIONS.md - electrical specs
- [ ] COMMON_ISSUES.md - solutions for each issue type

### Priority 2 (Referenced 3-4 times)
Expand these key development guides:
- [ ] DEVELOPMENT_SETUP.md - system-specific setup
- [ ] BUILD_SYSTEM.md - build procedures for each platform
- [ ] TESTING_GUIDE.md - framework and execution
- [ ] DEBUGGING.md - tool-specific procedures

### Priority 3 (Referenced 1-2 times)
Complete hardware module-specific documentation:
- [ ] Bearing.md (GNSS module)
- [ ] Borealis.md (environment module)
- [ ] Torrent.md (power delivery module)
- [ ] [11 other module-specific files - see 02_Hardware_Modules/README.md]

### Priority 4 (Supporting Documentation)
Additional content that enhances but isn't critical:
- [ ] CONFIG_FILES.md - Configuration reference
- [ ] API_DOCUMENTATION.md - REST API reference
- [ ] MQTT_PROTOCOL.md - Protocol specifics

---

## Documentation Quality Metrics

| Metric | Status | Notes |
|--------|--------|-------|
| All Core Principles enforced | ✅ Complete | CORE_PRINCIPLES.md + CONTRIBUTING.md |
| Cloud optionality clear | ✅ Complete | Every architecture doc emphasizes local first |
| Safety requirements documented | ✅ Complete | Wired CAN control mandatory in CONTRIBUTING |
| Link structure (broken links identified) | ✅ Complete | All links mapped, placeholders created |
| Architecture documentation | ✅ Complete | 5 comprehensive architecture files |
| Development guidelines | ✅ Complete | 6 files covering setup, build, test, debug |
| Deployment procedures | ✅ Complete | 5 files covering all deployment phases |
| Troubleshooting guides | ✅ Complete | 6 organized troubleshooting categories |
| Reference materials | ✅ Complete | 7 reference files with frameworks |

---

## Statistics

- **Total Files Created**: 46 (43 markdown + 1 DBC + 2 new)
- **Files with Full Content**: 26
- **Files with Complete Structure/Placeholders**: 20
- **Total Documentation Pages** (estimated): 80-100 pages
- **Cross-references**: 100+
- **Principle Enforcement Points**: 15+ (in CONTRIBUTING.md and CORE_PRINCIPLES.md)
- **Deployment Checklists**: 1 comprehensive (DEPLOYMENT_CHECKLIST.md)
- **Code Examples Documented**: Multiple (in setup and deployment guides)

---

## How to Use This Documentation

1. **First Time Users**: Start with README.md → QUICK_START.md
2. **Architects**: Read CORE_PRINCIPLES.md → 01_Architecture/PLATFORM_OVERVIEW.md
3. **Developers**: CONTRIBUTING.md → 07_Development/DEVELOPMENT_SETUP.md
4. **Operations**: 08_Deployment/README.md → DEPLOYMENT_CHECKLIST.md
5. **Troubleshooting**: 09_Troubleshooting/COMMON_ISSUES.md
6. **Reference Lookup**: 10_Reference/GLOSSARY.md + specific reference docs

---

## Notes

- All created files are production-ready scaffolding with clear content organization
- "NEEDS TO BE COMPLETED" sections guide future writers on what belongs in each section
- All links between documents are properly configured for future content
- Core principles are front-and-center to prevent architectural violations
- Placeholder files follow the same structure as completed files for consistency

---

**Status**: Documentation framework complete and ready for incremental content population.
