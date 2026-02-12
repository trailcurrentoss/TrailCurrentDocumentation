# CAN Bus Reference

Complete reference for CAN message formats, IDs, and protocols used in TrailCurrent.

## Overview

- **Protocol**: Controller Area Network (CAN 2.0)
- **Bitrate**: 500 kbps or 1 Mbps (configurable per network)
- **Message Format**: Standard 11-bit identifiers
- **Termination**: 120Ω resistors at both ends of bus

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

| Range | Category | MQTT Topic | Description |
|-------|----------|-----------|-------------|
| 0x000-0x0FF | GPS & Location | tc/gps/* | Location, heading, speed |
| 0x100-0x1FF | Temperature | tc/temp/* | Temperature sensors |
| 0x200-0x2FF | Power Control | tc/power/* | Power, voltage, current |
| 0x300-0x3FF | Status & Telemetry | tc/status/* | System status, diagnostics |
| 0x400-0x4FF | User Interface | tc/ui/* | Buttons, displays, controls |
| 0x500-0x5FF | Gateway/Bridge | tc/gateway/* | MQTT gateway messages |
| 0x600-0x6FF | External Systems | tc/external/* | External subsystems |
| 0x700-0x7FF | Reserved | Reserved | Future use |
| 0x800-0x8FF | Diagnostic/Debug | tc/debug/* | Debug and diagnostics |

## Module-Specific Messages

### GPS Module (0x000-0x00F)

**NEEDS TO BE COMPLETED** - Document GPS message formats, latitude/longitude encoding, precision, update rates

### Temperature Sensor (0x100-0x10F)

**NEEDS TO BE COMPLETED** - Document temperature message formats, sensor types, precision, units

### Power Control Module (0x200-0x20F)

**NEEDS TO BE COMPLETED** - Document power status, voltage, current, state messages

### Status & Telemetry (0x300-0x30F)

**NEEDS TO BE COMPLETED** - Document device status, error codes, diagnostic messages

### User Interface (0x400-0x40F)

**NEEDS TO BE COMPLETED** - Document button press messages, display control, user input

### Gateway Messages (0x500-0x50F)

**NEEDS TO BE COMPLETED** - Document MQTT gateway protocol, sync messages

### External Systems (0x600-0x60F)

**NEEDS TO BE COMPLETED** - Document external system control messages

## Bit Encoding Conventions

**NEEDS TO BE COMPLETED** - Document:
- Byte ordering (big-endian vs little-endian)
- Signed/unsigned integer encoding
- Floating-point encoding
- Boolean flags
- Enumeration values

## Error Codes

**NEEDS TO BE COMPLETED** - Document standard error codes used across all modules

## Message Timing

**NEEDS TO BE COMPLETED** - Document:
- Typical message frequencies
- Priority levels
- Timeout values
- Retry strategies

## Examples

**NEEDS TO BE COMPLETED** - Provide CAN message examples for:
- Each module type
- Common operations
- Error conditions
- Multi-message sequences

## Tools & Testing

**NEEDS TO BE COMPLETED** - Document:
- CAN monitoring tools
- Message capture/replay
- Testing procedures
- Debugging techniques

---

## Related Documentation

- [GLOSSARY.md](GLOSSARY.md) - CAN bus terminology
- [GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md) - CAN transceiver pin assignments
- [MQTT_TOPICS.md](MQTT_TOPICS.md) - MQTT mapping of CAN messages
- [../02_Hardware_Modules/](../02_Hardware_Modules/) - Module-specific details
