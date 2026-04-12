# Multi-Instance Module Pattern

Some TrailCurrent modules support multiple physical units on the same CAN bus. Each instance gets a unique address at build time, which determines its CAN message IDs. This document describes the pattern for implementing multi-instance support in Headwaters and the PWA.

## Currently Supported

| Module | Max Instances | Toggle CAN IDs | Status CAN IDs | Build Flag |
|--------|--------------|-----------------|-----------------|------------|
| Switchback | 3 | 0x25, 0x26, 0x27 | 0x28, 0x29, 0x2A | `SWITCHBACK_ADDRESS` |
| Torrent | 3 | 0x18, 0x19, 0x1A (toggle) / 0x15, 0x16, 0x17 (brightness) | 0x1B, 0x1C, 0x1D | `TORRENT_ADDRESS` |
| Picket | 8 | N/A (input only) | 0x0A-0x11 | `PICKET_ADDRESS` |
| Tapper | 6 (2 targets x 3 addr) | Uses target device's toggle IDs | Uses target device's status IDs | `TARGET_DEVICE` + `DEVICE_INSTANCE` |

## How It Works

### 1. Firmware — Build-Time Address

Each instance is compiled with a unique address via CMake build flag:

```bash
# Switchback address 1 (default is 0)
idf.py build -DSWITCHBACK_ADDRESS=1

# Torrent address 2
idf.py build -DTORRENT_ADDRESS=2
```

This shifts the CAN IDs: address 0 uses base IDs, address 1 uses base+1, etc. Each module repo includes a `build-all.sh` that builds all variants and produces `{type}_addr{N}.bin` files (see [Firmware Build, Release, and Deployment](#firmware-build-release-and-deployment) below).

### 2. Discovery — `canid` TXT Record

During mDNS discovery, each module advertises its computed CAN TX ID:

```
type=switchback  addr=1  canid=0x29  fw=1.0.0
```

The `canid` tells Headwaters which CAN status messages belong to this specific instance. For multi-ID devices (Torrent sends on 4 IDs), `canid` reports the primary status ID — Headwaters can compute the others from the `addr` and known offset scheme.

### 3. Channel Sync — Light ID Numbering

Each module type has a channel sync service that creates entries in the `lights` collection:

```
Light ID = ID_BASE + (instanceIndex * CHANNELS_PER_MODULE) + channel
```

For Switchback (`SWITCHBACK_ID_BASE = 100`, `CHANNELS_PER_MODULE = 8`):
- Instance 0: IDs 101-108
- Instance 1: IDs 109-116
- Instance 2: IDs 117-124

Each light entry stores:
- `relay_channel`: 0-7 (instance-relative, used as CAN payload byte)
- `relay_instance`: 0, 1, or 2 (used to compute CAN ID)
- `source`: `'switchback'` (routes toggle logic)

### 4. Toggle Command Routing

When the UI toggles a relay:

```
UI click → PUT /api/lights/109
         → lights.js finds light {relay_channel: 0, relay_instance: 1, source: 'switchback'}
         → mqttService.publishRelayToggle(0, 1)
         → can-bridge.sendRelayToggle(mqtt, 0, 1)
         → CAN ID 0x026 [0x00]   (0x025 + instance 1, channel 0)
```

### 5. Status Parsing

The CAN bridge maps status CAN IDs to global relay numbers:

```javascript
const SWITCHBACK_RELAY_OFFSET = { '0x028': 0, '0x029': 8, '0x02a': 16 };
```

CAN 0x029 byte0 bit 3 = relay 12 → MQTT `local/relays/12/status` → light ID 112.

### 6. All-On / All-Off

The all command must be sent to each active instance separately:

```javascript
const instances = await lights.distinct('relay_instance', { source: 'switchback' });
for (const instance of instances) {
    mqttService.publishRelayAllCommand(state, instance);
}
```

Each instance receives `CAN 0x025+N [0x08, state]`.

## Firmware Build, Release, and Deployment

Multi-address modules must produce one firmware binary per variant. Each repo includes a `build-all.sh` script.

**Single-dimension modules** (address only) use the naming convention `{type}_addr{N}.bin`:

```bash
cd TrailCurrentTorrent
./build-all.sh
# Produces: build/torrent_addr0.bin, torrent_addr1.bin, torrent_addr2.bin
```

**Two-dimension modules** (target device + address) like Tapper use `{type}_{target}_addr{N}.bin`:

```bash
cd TrailCurrentTapper
./build-all.sh
# Produces 6 binaries:
#   build/tapper_torrent_addr0.bin .. tapper_torrent_addr2.bin
#   build/tapper_switchback_addr0.bin .. tapper_switchback_addr2.bin
```

### Creating a GitHub Release

All address variants must be uploaded as release assets:

```bash
gh release create v1.0.0 \
  build/torrent_addr0.bin \
  build/torrent_addr1.bin \
  build/torrent_addr2.bin \
  --repo trailcurrentoss/TrailCurrentTorrent \
  --title "v1.0.0" \
  --notes "Firmware release v1.0.0"
```

### How the Deployment System Uses These

| System | Behavior |
|--------|----------|
| `fetch-firmware.sh` | `MULTI_ADDR`: downloads `{type}_addr{N}.bin`. `MULTI_TARGET_ADDR`: downloads `{type}_{target}_addr{N}.bin` for each target+address |
| `deploy.sh` | Reads `type`, `addr`, and `target` from MongoDB. Tries `{type}_{target}_addr{addr}.bin` first, then `{type}_addr{addr}.bin`, then `{type}.bin` |
| Headwaters UI (OTA) | Same resolution order as deploy.sh, using the module's `target` field from discovery |
| Web installer (flash.html) | Modules with `addresses` show an address dropdown; modules with `variants` show a target+address dropdown (e.g. Tapper) |
| Deployment ZIP | Includes all variants automatically (the `firmware/` directory is copied as-is) |

### Directory Structure on Headwaters

```
firmware/wired/torrent/
├── torrent_addr0.bin
├── torrent_addr1.bin
└── torrent_addr2.bin
firmware/wired/tapper/
├── tapper_torrent_addr0.bin
├── tapper_torrent_addr1.bin
├── tapper_torrent_addr2.bin
├── tapper_switchback_addr0.bin
├── tapper_switchback_addr1.bin
└── tapper_switchback_addr2.bin
```

After `deploy.sh` copies firmware to `data/firmware/` for the UI, the backend OTA route resolves firmware via `resolveFirmwareFile(type, addr, target)` which tries `{type}_{target}_addr{addr}.bin`, then `{type}_addr{addr}.bin`, then `{type}.bin`.

### Discovery and the `target` Field

Tapper advertises a `target` mDNS TXT record (e.g. `target=torrent`) during discovery. The host-side `discovery-mdns.py` captures this and stores it in the module record in MongoDB. The `target` field is used during OTA to resolve the correct firmware binary. Modules without a `target` field (all non-Tapper modules) are unaffected.

## Adding Multi-Instance Support for a New Module Type

Follow this checklist (using Torrent as a future example):

### Firmware Build and Release
- [ ] Add `build-all.sh` to the firmware repo (iterate addresses 0..max, build each, copy to `{type}_addr{N}.bin`)
- [ ] Add the module to `MULTI_ADDR` in Headwaters `fetch-firmware.sh` (e.g., `"torrent|2"`)
- [ ] Add `addresses: N` to the module entry in `flash.html` (web installer)
- [ ] Create a GitHub release with all address-specific binaries as assets

### CAN Bridge (`can-bridge.js`)
- [ ] Add offset mapping for status CAN IDs (e.g., `TORRENT_STATUS_OFFSET = { '0x01b': 0, '0x01c': 8, '0x01d': 16 }`)
- [ ] Add instance parameter to command functions (toggle, brightness)
- [ ] Compute CAN ID as `BASE + instance`

### Channel Sync Service (new or existing `*-channel-sync.js`)
- [ ] Filter enabled modules of this type, sorted by hostname
- [ ] Create light entries with `ID_BASE + (instanceIndex * 8) + channel`
- [ ] Store `relay_instance` (or equivalent) for CAN ID routing
- [ ] Store `relay_channel` as instance-relative (0-7)
- [ ] Clean up orphaned entries when modules are disabled

### Lights API (`lights.js`)
- [ ] Route toggle/brightness commands using `source` field
- [ ] Pass instance to MQTT publish functions
- [ ] Handle all-on/all-off per instance

### MQTT Service (`mqtt.js`)
- [ ] Accept and forward instance parameter in publish functions
- [ ] Map status MQTT topics to light IDs using base offset

### Frontend
- [ ] No changes needed — the home page renders all lights from `GET /api/lights`
- [ ] LightButton component already handles `source` field for UI differences (e.g., hiding brightness for switchback)

## Related Files

| File | Purpose |
|------|---------|
| `containers/backend/src/services/switchback-channel-sync.js` | Reference implementation |
| `containers/backend/src/services/pdm-channel-sync.js` | Torrent equivalent (single-instance currently) |
| `containers/backend/src/services/can-bridge.js` | CAN ID parsing and command routing |
| `containers/backend/src/routes/lights.js` | API command routing by source |
| `containers/backend/src/mqtt.js` | MQTT relay status → WebSocket broadcast |
| `TrailCurrentDocumentation/10_Reference/CAN_BUS_REFERENCE.md` | Full CAN ID allocation |
| `TrailCurrentDocumentation/TrailCurrent.dbc` | Machine-readable CAN database |
