# Task: Persist Discovery State to NVS Across All MCU Modules

## Problem

When a user adds devices to the CAN bus one at a time and runs discovery for each, previously-discovered devices re-enter discovery mode on every CAN 0x02 trigger. They connect to WiFi, advertise mDNS, and wait 3 minutes — even though Headwaters already knows about them. This causes race conditions where new devices can't be found because already-discovered devices are competing.

The `s_confirmed` flag in `discovery.c` is RAM-only. It's lost after the discovery task ends, so on the next CAN 0x02 broadcast (meant for a newly-connected device), every previously-discovered device enters discovery mode again.

## Solution

Two coordinated changes across ALL firmware modules:

### 1. Save "discovered" state to NVS after HTTP confirmation

When Headwaters calls `/discovery/confirm` on a device, save `discovered = true` to NVS. On boot, load this flag. On CAN 0x02, check the flag — if the device is already discovered, ignore the trigger.

### 2. Implement CAN 0x03 (DiscoveryReset) as a broadcast NVS wipe

CAN ID 0x03 is already defined in the DBC (`DiscoveryReset`) and already plumbed in Headwaters (`publishDiscoveryReset()` in mqtt.js, `POST /api/discovery/reset` route, `resetModuleDiscovery()` in api.js). But no firmware module handles it.

CAN 0x03 MUST be a **broadcast with no payload (DLC 0)**. When received, ALL devices clear their entire NVS and restart. This is the nuclear recovery option for when something goes wrong (e.g., a device saved "discovered" to NVS but Headwaters failed to save it to MongoDB — now the device is invisible and Headwaters doesn't know its hostname to target it).

**IMPORTANT:** The DBC currently defines 0x03 as MAC-targeted (3 bytes). It must be changed to broadcast (0 bytes). Headwaters' `publishDiscoveryReset()` currently sends 3 MAC bytes — it must be changed to send an empty payload.

## Why CAN 0x03 is required in this change

Without 0x03, there is no way to recover from a split-brain state where a device thinks it's discovered but Headwaters doesn't know about it. The device will ignore all future discovery triggers and is unreachable without physical access. This was the exact roadblock that prevented NVS persistence from being implemented previously. Both changes MUST ship together.

## All 11 MCU projects that need changes

Every project is under `/media/dave/extstorage/TrailCurrent/Product/` and follows the same pattern. All have `main/discovery.c`, `main/discovery.h`, and a CAN dispatch location.

| Project | CAN ID defined in | CAN dispatch in | Credential API |
|---------|-------------------|-----------------|----------------|
| TrailCurrentAftline | `ota.h` | `main.c` (if/else) | `ota_has_credentials()` / `ota_get_hostname()` |
| TrailCurrentBearing | `main.c` (local #define) | `main.c` (if/else) | `wifi_config_has_credentials()` / `wifi_config_get_hostname()` |
| TrailCurrentBorealis | `ota.h` | `main.c` (if/else) | `ota_has_credentials()` / `ota_get_hostname()` |
| TrailCurrentMilepost | `ota.h` | `main.c` (if/else) | `ota_has_credentials()` / `ota_get_hostname()` |
| TrailCurrentPicket | `main.c` (local #define) | `main.c` (if/else) | `wifi_config_has_credentials()` / `wifi_config_get_hostname()` |
| TrailCurrentPlateau | `main.c` (local #define) | `main.c` (if/else) | `wifi_config_has_credentials()` / `wifi_config_get_hostname()` |
| TrailCurrentSolstice | `ota.h` | `main.c` (if/else) | `ota_has_credentials()` / `ota_get_hostname()` |
| TrailCurrentSwitchback | `board.h` | `can_handler.c` (switch/case) | `wifi_config_has_credentials()` / `wifi_config_get_hostname()` |
| TrailCurrentTapper | `main.c` (local #define) | `main.c` (if/else) | `wifi_config_has_credentials()` / `wifi_config_get_hostname()` |
| TrailCurrentTorrent | `main.c` (local #define) | `main.c` (if/else) | `wifi_config_has_credentials()` / `wifi_config_get_hostname()` |

**Note on Bearing:** Its discovery trigger CAN ID is named `CAN_ID_DISCOVERY` (not `CAN_ID_DISCOVERY_TRIGGER`). Handle this when adding the reset ID.

## Exact changes per project

### A. `main/discovery.c` (all 11 projects — identical change)

The `confirm_handler` and `discovery_handle_trigger` functions are identical across all projects. The `discovery.c` files differ only in module-specific TXT record content (type, canid, addr) which is unrelated to these changes.

**1. Add `#include "nvs.h"` to the includes at the top of the file.**

**2. Add NVS namespace, key, and cached state after the existing state variables:**

```c
static volatile bool s_confirmed = false;
static volatile bool s_discovery_running = false;

#define NVS_NS_DISCOVERY    "discovery"
#define NVS_KEY_DISCOVERED  "discovered"

static bool s_discovered = false;
```

**3. Add NVS helper functions (before `discovery_mdns_start`):**

```c
static void discovery_load_state(void)
{
    nvs_handle_t h;
    if (nvs_open(NVS_NS_DISCOVERY, NVS_READONLY, &h) != ESP_OK) return;
    uint8_t val = 0;
    if (nvs_get_u8(h, NVS_KEY_DISCOVERED, &val) == ESP_OK) {
        s_discovered = (val != 0);
    }
    nvs_close(h);
}

static void discovery_save_state(void)
{
    nvs_handle_t h;
    if (nvs_open(NVS_NS_DISCOVERY, NVS_READWRITE, &h) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open NVS for discovery write");
        return;
    }
    nvs_set_u8(h, NVS_KEY_DISCOVERED, 1);
    nvs_commit(h);
    nvs_close(h);
    s_discovered = true;
    ESP_LOGI(TAG, "Discovery state saved — device is now discovered");
}
```

**4. Update `discovery_init()`:**

```c
void discovery_init(void)
{
    discovery_load_state();
    if (s_discovered) {
        ESP_LOGI(TAG, "Device already discovered — will ignore CAN 0x02 triggers");
    } else {
        ESP_LOGI(TAG, "Device not yet discovered — will respond to CAN 0x02 trigger");
    }
}
```

**5. Update `confirm_handler()` — add NVS save after setting s_confirmed:**

```c
static esp_err_t confirm_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Discovery confirmed by Headwaters");
    httpd_resp_sendstr(req, "confirmed\n");
    s_confirmed = true;
    discovery_save_state();
    return ESP_OK;
}
```

**6. Update `discovery_handle_trigger()` — add s_discovered check as the first guard:**

```c
void discovery_handle_trigger(void)
{
    if (s_discovered) {
        ESP_LOGD(TAG, "Already discovered — ignoring discovery trigger");
        return;
    }
    if (s_discovery_running) {
        // ... rest unchanged
```

**7. Add new public function `discovery_handle_reset()`:**

```c
void discovery_handle_reset(void)
{
    ESP_LOGW(TAG, "Discovery reset received — erasing NVS and restarting");
    nvs_flash_erase();
    esp_restart();
}
```

This erases ALL of NVS (WiFi credentials, calibration offsets, discovery state — everything) and reboots. This is intentionally destructive — it's the nuclear recovery option. After reboot, devices will need WiFi credentials re-provisioned before they can be discovered again.

### B. `main/discovery.h` (all 11 projects)

Add the reset function declaration:

```c
/**
 * Handle a CAN discovery reset broadcast (ID 0x03).
 * Erases all NVS and restarts the device.
 */
void discovery_handle_reset(void);
```

### C. CAN ID definition (varies by project)

Add `#define CAN_ID_DISCOVERY_RESET 0x03` in the same location where `CAN_ID_DISCOVERY_TRIGGER` is defined:

- **Aftline, Borealis, Milepost, Solstice**: add to `ota.h`
- **Switchback**: add to `board.h`
- **Bearing, Picket, Plateau, Tapper, Torrent**: add to `main.c` next to the existing local `#define`
- **Bearing special case**: its existing define is `CAN_ID_DISCOVERY` not `CAN_ID_DISCOVERY_TRIGGER` — name the new one `CAN_ID_DISCOVERY_RESET` to stay consistent with all other projects

### D. CAN dispatch (varies by project)

Add a handler for CAN 0x03 in the receive loop, right after the discovery trigger handler:

**For Switchback** (`can_handler.c`, switch/case):
```c
case CAN_ID_DISCOVERY_RESET:
    discovery_handle_reset();
    break;
```

**For all other projects** (`main.c`, if/else chain):
```c
} else if (msg.identifier == CAN_ID_DISCOVERY_RESET) {
    discovery_handle_reset();
}
```

### E. Include for `nvs_flash.h`

The `discovery_handle_reset()` function calls `nvs_flash_erase()` which is in `nvs_flash.h`. Check if `discovery.c` already includes it (some do via `ota.h` chain, some don't). Add `#include "nvs_flash.h"` if not already included.

The `discovery_save_state()` and `discovery_load_state()` functions use `nvs.h` which is a separate header. Both `nvs.h` and `nvs_flash.h` may need to be added.

## DBC Update

File: `/media/dave/extstorage/TrailCurrent/Product/TrailCurrentDocumentation/TrailCurrent.dbc`

Change the existing DiscoveryReset definition from MAC-targeted to broadcast:

**Before:**
```
BO_ 3 DiscoveryReset: 3 Headwaters
 SG_ MacAddressByte1 : 7|8@0+ (1,0) [0|255] "" Borealis,...
 SG_ MacAddressByte2 : 15|8@0+ (1,0) [0|255] "" Borealis,...
 SG_ MacAddressByte3 : 23|8@0+ (1,0) [0|255] "" Borealis,...
```

**After:**
```
BO_ 3 DiscoveryReset: 0 Headwaters
```

Update the comment to:
```
CM_ BO_ 3 "Discovery reset broadcast. Erases all NVS on every module and triggers a restart. All devices lose WiFi credentials, calibration data, and discovery state. After reboot, devices must be re-provisioned with WiFi credentials (CAN 0x01) before discovery (CAN 0x02) can succeed. This is a recovery mechanism for when devices and Headwaters are out of sync.";
```

## Verification per project

After flashing each module:

1. **Boot with clean NVS** → log: "Device not yet discovered — will respond to CAN 0x02 trigger"
2. **Run discovery, confirm via Headwaters** → log: "Discovery state saved — device is now discovered"
3. **Send CAN 0x02 again** → log (at debug level): "Already discovered — ignoring discovery trigger" — device does NOT connect to WiFi
4. **Reboot** → log: "Device already discovered — will ignore CAN 0x02 triggers" — NVS persists
5. **Send CAN 0x03** → log: "Discovery reset received — erasing NVS and restarting" — device reboots, comes back as undiscovered

## DO NOT modify any other behavior

- Do not change the discovery timeout, mDNS advertisement, HTTP server, or WiFi connection logic
- Do not change OTA or WiFi provisioning
- Do not add any new CAN messages beyond 0x03
- Do not add periodic/heartbeat mechanisms
- The ONLY changes are: NVS read/write for discovered state, the s_discovered guard in discovery_handle_trigger, and the CAN 0x03 handler
