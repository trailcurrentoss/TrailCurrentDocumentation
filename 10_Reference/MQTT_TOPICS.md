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

**NEEDS TO BE COMPLETED** - Define QoS for:
- Critical system messages (QoS 2)
- Important data (QoS 1)
- Telemetry (QoS 0)
- Command messages
- Response messages

## Retained Messages

**NEEDS TO BE COMPLETED** - Document which topics should retain values:
- Device status
- Configuration
- Last known values
- Retention policies

## Subscription Patterns

**NEEDS TO BE COMPLETED** - Document:
- Client subscription patterns
- Wildcard usage
- Filter combinations
- Performance considerations

## Local MQTT vs Cloud MQTT

**NEEDS TO BE COMPLETED** - Document:
- Topic differences between local and cloud
- Synchronization strategy
- Conflict resolution
- Bridge configuration

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
