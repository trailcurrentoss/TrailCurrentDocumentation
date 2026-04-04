# Multi-Instance Module Pattern

Some TrailCurrent modules support multiple physical units on the same CAN bus. Each instance gets a unique address at build time, which determines its CAN message IDs. This document describes the pattern for implementing multi-instance support in Headwaters and the PWA.

## Currently Supported

| Module | Max Instances | Toggle CAN IDs | Status CAN IDs | Build Flag |
|--------|--------------|-----------------|-----------------|------------|
| Switchback | 3 | 0x25, 0x26, 0x27 | 0x28, 0x29, 0x2A | `DEVICE_INSTANCE` |
| Torrent | 3 | 0x18, 0x19, 0x1A (toggle) / 0x15, 0x16, 0x17 (brightness) | 0x1B, 0x1C, 0x1D | `TORRENT_ADDRESS` |
| Picket | 8 | N/A (input only) | 0x0A-0x11 | `PICKET_ADDRESS` |
| Tapper | 3 | Uses target device's toggle IDs | N/A (no status) | `DEVICE_INSTANCE` |

## How It Works

### 1. Firmware — Build-Time Address

Each instance is compiled with a unique address via CMake build flag:

```cmake
# Switchback instance 1 (default is 0)
add_compile_definitions(DEVICE_INSTANCE=1)
```

This shifts the CAN IDs: instance 0 uses base IDs, instance 1 uses base+1, etc.

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

## Adding Multi-Instance Support for a New Module Type

Follow this checklist (using Torrent as a future example):

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
