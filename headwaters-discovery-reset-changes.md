# Headwaters Changes: Discovery Reset (CAN 0x03) Button

## Context

All MCU modules now persist their "discovered" state to NVS. When Headwaters confirms a device via HTTP `/discovery/confirm`, the device saves `discovered = true` to NVS and will ignore future CAN 0x02 discovery triggers.

CAN 0x03 is the recovery mechanism. It is a **broadcast with no payload** that tells ALL connected MCUs to erase their entire NVS and restart. After restart, devices need WiFi credentials re-provisioned (CAN 0x01) before discovery (CAN 0x02) can work.

The backend and API client already have the plumbing for this — but the payload needs to change from MAC-targeted to broadcast, and a UI button needs to be added.

## Change 1: `containers/backend/src/mqtt.js` — Change `publishDiscoveryReset()` to broadcast

**Current code (line ~821):**
```javascript
publishDiscoveryReset(hostname) {
    // ... parses hostname into 3 MAC bytes ...
    return this.publishCanMessage(0x03, macBytes);
}
```

**New code:**
```javascript
publishDiscoveryReset() {
    if (!this.connected) {
        console.warn('MQTT not connected, cannot publish discovery reset');
        return false;
    }
    console.log('[Discovery] Broadcasting CAN 0x03 discovery reset to ALL modules');
    return this.publishCanMessage(0x03, []);
}
```

Remove the `hostname` parameter entirely. Send empty data array (DLC 0).

## Change 2: `containers/backend/src/routes/discovery.js` — Update reset route

**Current code (line ~175):**
```javascript
router.post('/reset', (req, res) => {
    const { hostname } = req.body;
    if (!hostname || typeof hostname !== 'string') {
        return res.status(400).json({ error: 'hostname is required' });
    }
    const success = mqttService.publishDiscoveryReset(hostname);
    // ...
});
```

**New code:**
```javascript
router.post('/reset', (req, res) => {
    try {
        const success = mqttService.publishDiscoveryReset();
        if (!success) {
            return res.status(503).json({ error: 'Failed to send discovery reset' });
        }
        // Clear all modules from the database since devices will lose their state
        // (optional — depends on whether you want Headwaters to also forget)
        res.json({ success: true, message: 'Discovery reset broadcast sent — all modules will restart' });
    } catch (error) {
        console.error('Error sending discovery reset:', error);
        res.status(500).json({ error: 'Failed to send discovery reset' });
    }
});
```

No longer requires `hostname` in the request body.

## Change 3: `containers/frontend/public/js/api.js` — Update client method

**Current code (line ~329):**
```javascript
static async resetModuleDiscovery(hostname) {
    return this.request('/discovery/reset', {
        method: 'POST',
        body: JSON.stringify({ hostname })
    });
}
```

**New code:**
```javascript
static async resetAllDiscovery() {
    return this.request('/discovery/reset', {
        method: 'POST'
    });
}
```

Renamed to `resetAllDiscovery()` since it affects all devices, not a single module. No body needed.

## Change 4: Add UI button

Add a "Reset All Modules" button to the discovery page/section of the frontend. This is a destructive action — it should:

- Have a confirmation dialog: "This will erase all NVS on every connected module and restart them. WiFi credentials will need to be re-provisioned. Continue?"
- Call `Api.resetAllDiscovery()`
- Display a status message: "Reset broadcast sent — modules are restarting"
- Optionally clear the `mcu_modules` array in MongoDB since the devices will come back as undiscovered

## Important Notes

- CAN 0x03 erases ALL NVS on every module — not just the discovery flag. WiFi credentials, calibration offsets, and any other NVS-stored data are lost.
- After a reset, the normal discovery flow is: send WiFi credentials (0x01), then discovery trigger (0x02). The UI's "Start Discovery" button already does this.
- This is a last-resort recovery tool, not a routine operation. Label it accordingly in the UI.
