# MQTT Topics Reference

Complete reference for all MQTT topics, message formats, and publish/subscribe patterns used in TrailCurrent.

## Overview

**NEEDS TO BE COMPLETED** - Introduction to MQTT in TrailCurrent, QoS levels, retention policies

## Topic Hierarchy

```
tc/                              # Root namespace
├── gps/                         # GPS and location data
├── temp/                        # Temperature sensors
├── power/                       # Power system
├── status/                      # System status
├── ui/                          # User interface
├── gateway/                     # Gateway/bridge
├── external/                    # External systems
├── command/                     # Control commands
├── telemetry/                   # Telemetry data
├── device/                      # Device information
└── system/                      # System messages
```

## Standard Topics

### GPS Topics (tc/gps/*)

**NEEDS TO BE COMPLETED** - Document:
- `tc/gps/position` - Current location (lat/lon)
- `tc/gps/heading` - Compass heading
- `tc/gps/speed` - Current speed
- `tc/gps/altitude` - Elevation
- `tc/gps/satellites` - Satellite count
- `tc/gps/fix` - GPS fix status
- Message formats and payload examples

### Temperature Topics (tc/temp/*)

**NEEDS TO BE COMPLETED** - Document:
- `tc/temp/[location]/value` - Current temperature
- `tc/temp/[location]/unit` - Temperature unit (C/F)
- `tc/temp/[location]/min` - Minimum recorded
- `tc/temp/[location]/max` - Maximum recorded
- Sensor location naming conventions
- Payload formats

### Power Topics (tc/power/*)

**NEEDS TO BE COMPLETED** - Document:
- `tc/power/battery/voltage` - Battery voltage
- `tc/power/battery/current` - Current draw
- `tc/power/battery/soc` - State of charge
- `tc/power/solar/voltage` - Solar input voltage
- `tc/power/solar/current` - Solar input current
- `tc/power/load/[device]/state` - Device on/off state
- Message formats and units

### Status Topics (tc/status/*)

**NEEDS TO BE COMPLETED** - Document:
- `tc/status/device/[id]` - Device status
- `tc/status/system/uptime` - System uptime
- `tc/status/system/version` - Software version
- `tc/status/network/connection` - Connection status
- Error and warning messages

### Command Topics (tc/command/*)

**NEEDS TO BE COMPLETED** - Document:
- `tc/command/device/[id]/on` - Turn device on
- `tc/command/device/[id]/off` - Turn device off
- `tc/command/device/[id]/set` - Set device parameter
- `tc/command/system/reboot` - System reboot
- `tc/command/ota/update` - OTA update trigger
- Response/acknowledgment patterns

### Device Topics (tc/device/*)

**NEEDS TO BE COMPLETED** - Document:
- `tc/device/[id]/info` - Device information
- `tc/device/[id]/config` - Device configuration
- `tc/device/[id]/capabilities` - Device capabilities
- Device ID naming conventions
- Device registry topics

### System Topics (tc/system/*)

**NEEDS TO BE COMPLETED** - Document:
- `tc/system/time` - System time synchronization
- `tc/system/version` - Platform version
- `tc/system/config` - System configuration
- `tc/system/health` - System health status
- Heartbeat/keepalive messages

## Message Formats

### Standard Payload Format (JSON)

**NEEDS TO BE COMPLETED** - Document:
- Standard JSON structure
- Timestamp formats
- Unit conventions
- Value ranges
- Error response format

### Example Messages

**NEEDS TO BE COMPLETED** - Provide examples for:
- GPS position update
- Temperature reading
- Power status
- Device control command
- Status response
- Error message

## QoS Levels

| QoS | Used For | Topics |
|---|---|---|
| 0 (fire-and-forget) | Non-critical telemetry | `local/system/stats` |
| 1 (at-least-once) | All status updates, commands, config | All `local/*` and `rv/*` status and command topics |
| 2 (exactly-once) | Raw CAN frames | `can/inbound` only |

QoS 1 is the default for cloud bridge traffic. Combined with the 20-second heartbeat, this provides reliable delivery with automatic recovery from connection drops.

## Retained Messages

Only configuration topics use MQTT retain. Sensor data and status updates are transient.

| Topic | Retained | Reason |
|---|---|---|
| `local/config/pdm_channels` | Yes | New subscribers get channel config immediately |
| `local/config/system_sync` / `rv/config/system` | Yes | Full system snapshot for new connections |
| All status topics (`local/lights/*`, etc.) | No | Heartbeat provides state recovery |
| All command topics | No | Commands are one-shot actions |

## Subscription Patterns

**NEEDS TO BE COMPLETED** - Document:
- Client subscription patterns
- Wildcard usage
- Filter combinations
- Performance considerations

## Local MQTT vs Cloud MQTT

The system uses two separate MQTT brokers — a local broker on the vehicle and a cloud broker on Farwatch. The cloud bridge (`cloud-bridge.js`) selectively forwards data between them.

### Topic Namespaces

| Namespace | Broker | Purpose |
|---|---|---|
| `local/*` | Vehicle (Mosquitto, port 8883) | Real-time CAN bus data, internal messaging |
| `rv/*` | Farwatch cloud (Mosquitto, port 8883) | Remote monitoring and control |
| `can/inbound` | Vehicle | Raw CAN frames from hardware (QoS 2) |
| `can/outbound` | Vehicle | CAN commands to hardware |

### Local → Cloud Bridging (Status)

The cloud bridge forwards local status topics to the cloud with **change detection**, **tiered intervals**, and a **20-second heartbeat** to minimize cellular data usage. See [DATA_FLOW.md](../01_Architecture/DATA_FLOW.md) for full details.

| Local Topic | Cloud Topic | Tier | Interval |
|---|---|---|---|
| `local/lights/+/status` | `rv/lights/N/status` | Immediate | On change only |
| `local/relays/+/status` | `rv/relays/N/status` | Immediate | On change only |
| `local/thermostat/status` | `rv/thermostat/status` | Immediate | On change only |
| `local/energy/status` | `rv/energy/status` | Standard | 5s (threshold bypass) |
| `local/gps/latlon` | `rv/gps/latlon` | Standard | 5s (threshold bypass) |
| `local/gps/alt` | `rv/gps/alt` | Slow | 15s |
| `local/gps/details` | `rv/gps/details` | Slow | 15s |
| `local/airquality/status` | `rv/airquality/status` | Slow | 15s |
| `local/airquality/temphumid` | `rv/airquality/temphumid` | Slow | 15s |
| `local/level/tilt` | `rv/level/tilt` | Slow | 15s |
| `local/level/status` | `rv/level/status` | Slow | 15s |
| `local/system/stats` | `rv/system/stats` | Background | 30s |
| `local/config/pdm_channels` | `rv/config/pdm_channels` | Config | 5s, change-only |
| `local/config/system_sync` | `rv/config/system` | Config | 5s, retained |
| `local/gps/time` | *(not forwarded)* | Eliminated | — |

### Cloud → Local Bridging (Commands)

Commands from Farwatch flow to the vehicle with no rate limiting or filtering:

| Cloud Topic | Action |
|---|---|
| `rv/lights/N/command` | Toggle light N (CAN 0x018) |
| `rv/lights/N/brightness` | Set brightness for light N (CAN 0x015) |
| `rv/lights/all/command` | All lights on/off (CAN 0x018 broadcast) |
| `rv/relays/N/command` | Toggle relay N (CAN 0x025+) |
| `rv/relays/all/command` | All relays on/off |
| `rv/thermostat/command` | Pass through to `local/thermostat/command` |
| `rv/proximity/event` | Proximity zone transition → `local/proximity/event` |
| `rv/proximity/status` | Proximity status → `local/proximity/status` |

### Heartbeat & Desync Recovery

Every 20 seconds, the cloud bridge republishes all last-known state to the cloud broker. This is a safety net for connection-level failures — in normal operation, state changes are forwarded immediately via MQTT QoS 1 (at-least-once delivery).

On cloud broker reconnect, the bridge immediately publishes a forced heartbeat (all cached state) and triggers a full system config re-sync.

## Security

**NEEDS TO BE COMPLETED** - Document:
- Authentication requirements
- Topic ACLs
- Message encryption
- Credential management

## Tools & Debugging

**NEEDS TO BE COMPLETED** - Document:
- MQTT client tools (mosquitto_sub, etc.)
- Message monitoring
- Topic inspection
- Payload validation

---

## Related Documentation

- [CAN_BUS_REFERENCE.md](CAN_BUS_REFERENCE.md) - CAN message mapping
- [GLOSSARY.md](GLOSSARY.md) - MQTT terminology
- [../01_Architecture/](../01_Architecture/) - System architecture
- [../03_Vehicle_Compute/](../03_Vehicle_Compute/) - Edge compute configuration
