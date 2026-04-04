# SMS Notifications

TrailCurrent can send SMS alarm notifications when lights or relays change state
unexpectedly. Messages are sent via a GL-iNet cellular router (Spitz AX / GL-X3000)
over SSH.

## How It Works

```
Light/Relay state change detected (CAN bus -> MQTT -> backend)
        |
        v
  Alarm enabled?  ----no----> stop
        |
       yes
        v
  SMS enabled?  ----no----> stop
        |
       yes
        v
  Throttle check: sent < max in window?  ----no----> stop (logged)
        |
       yes
        v
  SSH to GL-iNet router -> sendsms command -> SMS delivered
```

### What Triggers an Alarm

Currently only **light and relay state changes** trigger alarm SMS:

- **Borealis (PDM) lights** -- CAN ID 0x01b status frames parsed by the CAN bridge
- **Switchback relays** -- CAN IDs 0x028/0x029/0x02a status frames

Other sensor data (GPS, air quality, energy, thermostat, leveling) does **not**
trigger alarms at this time.

### Throttle Mechanism

A global sliding-window throttle prevents SMS flooding. Two user-configurable values
control it:

| Setting | DB field | Default | Range |
|---------|----------|---------|-------|
| Max messages | `sms_max_messages` | 3 | 1 - 100 |
| Window (minutes) | `sms_throttle_window_minutes` | 60 | 1 - 1440 |

The backend tracks timestamps of recently sent SMS messages in memory. Before sending,
it prunes timestamps older than the window, then checks if the count is under the max.
The throttle is **global** -- it does not distinguish between light vs relay events.
All alarm SMS share one budget.

Example: with defaults of 3 messages per 60 minutes, the 4th alarm within an hour is
suppressed. Once the oldest message ages out of the window, a new slot opens.

## Configuration

### Settings Page (UI)

1. **Enable SMS** -- master toggle for SMS functionality
2. **Phone Number** -- destination number (include `+` country code for international)
3. **Router IP Address** -- GL-iNet router LAN IP (typically `192.168.8.1`)
4. **SSH Private Key** -- OpenSSH private key for root access to the router
5. **Max messages** -- how many SMS to allow in the throttle window
6. **Minutes** -- throttle window duration

All fields are saved together with the **Save SMS Settings** button.

The **Alarm** toggle on the Home page must also be enabled for notifications to fire.

### Database Fields (system_config collection, `_id: 'main'`)

| Field | Type | Description |
|-------|------|-------------|
| `alarm_enabled` | boolean | Master alarm toggle (Home page) |
| `sms_enabled` | boolean | SMS feature toggle (Settings page) |
| `sms_phone_number` | string | Destination phone number |
| `sms_router_ip` | string | GL-iNet router IP |
| `sms_ssh_key_encrypted` | string | AES-encrypted SSH private key |
| `sms_ssh_key_iv` | string | AES IV for SSH key |
| `sms_max_messages` | number | Throttle: max SMS per window |
| `sms_throttle_window_minutes` | number | Throttle: window size in minutes |

## GL-iNet Router Setup (Spitz AX / GL-X3000)

### Prerequisites

- Active cellular connection with SMS capability on the SIM card
- SSH access enabled on the router (Administration > SSH)
- SSH key pair generated and public key installed on the router

### How sendsms Works

The `sendsms` utility on GL-iNet routers (at `/usr/bin/sendsms`) uses `smstools3`.
It does **not** send SMS directly -- it creates a file in `/etc/spool/sms/outgoing/`
and the `smsd` daemon picks it up and sends it through the modem.

The message flow:

```
Backend (SSH) -> sendsms command -> creates file in outgoing spool
                                          |
                                    smsd daemon picks up file
                                          |
                                    sends via modem AT commands
                                          |
                                    moves file to sent/ or failed/
```

### Phone Number Format

The `sendsms` command takes a third argument for the number type. The backend
hardcodes `National` which works for US numbers with or without a `+` prefix.

Note: if messages fail silently, check the `failed/` spool directory on the router --
`sendsms` always exits 0 regardless of whether delivery succeeds.

### SMS Spool Directories

Messages pass through directories on the router:

| Directory | Purpose |
|-----------|---------|
| `/etc/spool/sms/outgoing/<bus>/` | Queued, waiting to send |
| `/etc/spool/sms/sent/<bus>/` | Successfully sent |
| `/etc/spool/sms/failed/<bus>/` | Failed to send |

The `<bus>` subdirectory (e.g. `2-1`) corresponds to the modem's USB bus identifier.

### Checking for Problems

SSH into the router and inspect the spool:

```bash
# Check for failed messages
ls /etc/spool/sms/failed/*/

# Read a failed message to see the error
cat /etc/spool/sms/failed/2-1/send_XXXXXX
# Look for the Fail_reason: line

# Check how many sent messages have accumulated
ls /etc/spool/sms/sent/*/ | wc -l

# Check if smsd is running
ps | grep smsd
```

### Cleaning Up Spool Directories

The `sent/` and `failed/` directories are **not automatically cleaned** by `smsd`.
Files accumulate indefinitely. Periodically clean them:

```bash
# Clear failed messages
rm /etc/spool/sms/failed/2-1/send_*

# Clear sent message history
rm /etc/spool/sms/sent/2-1/send_*
```

### smsd Configuration

The daemon config is at `/etc/smsd.conf` on the router. Key settings:

```ini
devices = GSM1
outgoing = /etc/spool/sms/outgoing
sent = /etc/spool/sms/sent
failed = /etc/spool/sms/failed

[GSM1]
device = /dev/ttyUSB3
baudrate = 115200
```

## Troubleshooting

### No SMS received but logs say "SMS sent"

The backend reports success when the SSH command exits 0 -- but `sendsms` always
exits 0 even if the message will fail. Check the router:

```bash
ssh root@192.168.8.1 'ls /etc/spool/sms/failed/*/'
```

If files are present, read one to see the `Fail_reason`.

**Common causes:**
- Wrong National/International type for the phone number format
- SIM card doesn't support SMS
- No cellular signal
- Modem device path changed (check `/etc/smsd.conf`)

### No alarm log lines in backend

The `[Alarm]` logs can be buried under high-frequency GPS/sensor data. Filter:

```bash
docker compose logs backend 2>&1 | grep -i alarm | tail -20
```

If there are zero alarm lines ever (even at startup), check:
- Is the CAN bridge initialized? Look for `[CAN Bridge] Subscribed to can/inbound`
- Are status frames arriving? Look for `Received light status` or relay state changes

### SMS flooding

If too many messages are being sent, reduce `sms_max_messages` on the Settings page.
The default of 3 per 60 minutes is conservative. Each light or relay state change
triggers one alarm call -- and with 8 relays, an "All Off" command can trigger up to
8 state changes rapidly.

### Test SMS works but alarm SMS doesn't (or vice versa)

Both use the same `executeRemoteSms()` function and same SSH credentials. If one
works and the other doesn't, the issue is likely in the alarm detection path
(state cache, config flags) rather than SMS delivery. Check:

```bash
docker compose logs backend 2>&1 | grep -i alarm | tail -20
```

Look for "skipped" messages which indicate a config issue (alarm not enabled, SMS
not enabled, missing credentials).

## Source Code

| File | Purpose |
|------|---------|
| `containers/backend/src/mqtt.js` | Alarm detection (state change) and throttled SMS dispatch |
| `containers/backend/src/routes/sms.js` | `executeRemoteSms()` -- SSH to router and run sendsms |
| `containers/backend/src/routes/system-config.js` | SMS config CRUD and validation |
| `containers/backend/src/services/can-bridge.js` | CAN frame parsing, publishes light/relay status to MQTT |
| `containers/backend/src/db/init.js` | DB migration for default throttle values |
| `containers/frontend/public/js/pages/settings.js` | SMS settings UI |
| `containers/frontend/public/js/pages/home.js` | Alarm toggle UI |
