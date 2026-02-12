# TrailCurrent Reference Documentation

Complete reference materials for the TrailCurrent platform.

## Overview

This section contains:
- Detailed specifications
- Protocol definitions
- Pin mappings and hardware details
- Glossary of terms
- External resources

## Reference Documents

### CAN Bus Reference
[CAN_BUS_REFERENCE.md](CAN_BUS_REFERENCE.md)

Details for all CAN messages:
- Message format and structure
- CAN IDs by module type
- Data payload definitions
- Message frequency and priority
- Example implementations

### MQTT Topics
[MQTT_TOPICS.md](MQTT_TOPICS.md)

MQTT topic hierarchy and definitions:
- Topic naming convention
- Message format (JSON)
- QoS levels
- Retained messages
- Subscription patterns

### GPIO Pin Mapping
[GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md)

Hardware pin assignments:
- ESP32 pin definitions per module
- CAN interface pins
- Sensor pins (I2C, SPI, analog)
- Control output pins
- Power and ground

### Hardware Specifications
[HARDWARE_SPECIFICATIONS.md](HARDWARE_SPECIFICATIONS.md)

Technical specifications:
- Microcontroller details
- Power requirements
- Voltage/current ratings
- Connector specifications
- Environmental operating ranges

### Glossary
[GLOSSARY.md](GLOSSARY.md)

Terminology and acronyms:
- CAN, MQTT, REST, WebSocket
- Module names and codes
- Common abbreviations
- Technical terms

### External Links
[EXTERNAL_LINKS.md](EXTERNAL_LINKS.md)

Resources outside this documentation:
- Official documentation links
- Third-party libraries
- Standards and specifications
- Tool downloads

## Quick Reference Tables

### CAN ID Allocation

```
0x000-0x0FF: GPS & Location        (tc/gps/*)
0x100-0x1FF: Temperature           (tc/temp/*)
0x200-0x2FF: Power Control         (tc/power/*)
0x300-0x3FF: Status & Telemetry    (tc/status/*)
0x400-0x4FF: User Interface        (tc/ui/*)
0x500-0x5FF: Gateway/Bridge        (tc/gateway/*)
0x600-0x6FF: External Systems      (tc/external/*)
0x700-0x7FF: Reserved
0x800-0x8FF: Diagnostic/Debug      (tc/debug/*)
```

### MQTT Topic Hierarchy

```
tc/                              # Root
├── gps/position               # GPS location
├── gps/status
├── temp/current_temp          # Temperature
├── temp/humidity
├── power/voltage              # Power metrics
├── power/current
├── status/[device]            # Device status
├── command/[device]           # Device commands
└── debug/[module]             # Debug output
```

### Voltage/Power Standards

```
Input Voltage:  12V or 24V (vehicle power)
Logic Level:    3.3V (ESP32)
Module Power:   1-2W typical, 5W peak
CAN Termination: 120Ω resistors
```

## Module Quick Reference

### All Modules
- **MCU**: ESP32 or ESP32-S3
- **Compiler**: GCC + IDF
- **Memory**: 4MB Flash, 320KB RAM
- **Communication**: CAN Bus primary, optional WiFi/BT
- **Bootloader**: USB UART or OTA

### Power Control Module (PCM)
- **CAN ID**: 0x200 (commands), 0x280 (status)
- **Outputs**: 4-8 relay channels
- **Inputs**: Status from other modules
- **Key Pins**: GPIO for relays, CAN pins

### GPS Module
- **CAN ID**: 0x100-0x10F
- **Interface**: Serial UART to GPS module
- **Frequency**: 1 Hz typical
- **Accuracy**: 5m typical (depends on receiver)

### Temperature Sensor
- **CAN ID**: 0x120-0x12F
- **Interfaces**: I2C or 1-Wire
- **Sensors**: DS18B20, DHT22, BME680
- **Range**: -40°C to +125°C typical

See [HARDWARE_SPECIFICATIONS.md](HARDWARE_SPECIFICATIONS.md) for complete specs.

## Documentation by Type

### For Hardware Design
- [GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md)
- [HARDWARE_SPECIFICATIONS.md](HARDWARE_SPECIFICATIONS.md)
- [02_Hardware_Modules/](../02_Hardware_Modules/)

### For Protocol Implementation
- [CAN_BUS_REFERENCE.md](CAN_BUS_REFERENCE.md)
- [MQTT_TOPICS.md](MQTT_TOPICS.md)
- [01_Architecture/NETWORK_TOPOLOGY.md](../01_Architecture/NETWORK_TOPOLOGY.md)

### For Integration
- [MQTT_TOPICS.md](MQTT_TOPICS.md)
- [04_Cloud_Application/Backend/API_REFERENCE.md](../04_Cloud_Application/Backend/API_REFERENCE.md)
- [EXTERNAL_LINKS.md](EXTERNAL_LINKS.md)

### For Troubleshooting
- [GLOSSARY.md](GLOSSARY.md)
- [09_Troubleshooting/COMMON_ISSUES.md](../09_Troubleshooting/COMMON_ISSUES.md)
- [GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md)

## Standards & Specifications

### CAN Bus
- ISO 11898-1 Standard
- 500 kbps or 1 Mbps speeds
- 8-byte frames
- 11-bit identifier

### MQTT
- MQTT 3.1.1 / 5.0 compliant
- Hierarchical topic structure
- QoS 0 and 1 support
- Retained messages

### REST API
- JSON request/response format
- Standard HTTP methods
- JWT authentication
- HTTP status codes

### IP Networking
- IPv4 (primary)
- DNS for service discovery
- DHCP for address assignment
- TLS 1.2+ for encryption

## Numeric References

### Message IDs (CAN)

| Device Type | Start | End | Count |
|-------------|-------|-----|-------|
| GPS | 0x100 | 0x10F | 16 |
| Temperature | 0x120 | 0x12F | 16 |
| Power | 0x200 | 0x2FF | 256 |
| Status | 0x300 | 0x3FF | 256 |
| UI | 0x400 | 0x4FF | 256 |
| Gateway | 0x500 | 0x5FF | 256 |

### Port Numbers

| Service | Port | Protocol |
|---------|------|----------|
| REST API | 3000 | HTTP (dev) |
| REST API | 443 | HTTPS (prod) |
| MQTT | 1883 | TCP |
| MQTT TLS | 8883 | TCP+TLS |
| WebSocket | 8080 | WS (dev) |
| WebSocket | 443 | WSS (prod) |
| PostgreSQL | 5432 | TCP (internal) |
| Redis | 6379 | TCP (internal) |

### Data Sizes

| Data Type | Bytes | Notes |
|-----------|-------|-------|
| CAN Frame | 8 | 64-bit payload |
| GPS coord | 4 | 32-bit float |
| Temperature | 2 | 16-bit fixed point |
| Timestamp | 4 | Unix seconds |
| Message ID | 1 | CAN ID byte |

## Common Patterns

### CAN Message Format
```
[ID (1 byte)] [TYPE (1 byte)] [DATA (6 bytes)]
- ID: Module CAN identifier
- TYPE: Message type/command
- DATA: Type-specific payload
```

### MQTT Payload Format
```json
{
  "timestamp": 1234567890,
  "device_id": "gps_001",
  "value": 47.2530,
  "unit": "degrees",
  "quality": 1
}
```

### API Response Format
```json
{
  "success": true,
  "data": { /* response data */ },
  "timestamp": 1234567890,
  "request_id": "req_abc123"
}
```

## Cross-Reference

### All CAN Devices
- See [02_Hardware_Modules/](../02_Hardware_Modules/) for each module
- See [CAN_BUS_REFERENCE.md](CAN_BUS_REFERENCE.md) for CAN details

### All MQTT Topics
- See [MQTT_TOPICS.md](MQTT_TOPICS.md) for full hierarchy
- See [01_Architecture/DATA_FLOW.md](../01_Architecture/DATA_FLOW.md) for examples

### All API Endpoints
- See [04_Cloud_Application/Backend/API_REFERENCE.md](../04_Cloud_Application/Backend/API_REFERENCE.md)

### All Pins
- See [GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md) for each module

## Abbreviations & Acronyms

| Acronym | Meaning | Context |
|---------|---------|---------|
| CAN | Controller Area Network | Hardware communication |
| MQTT | MQ Telemetry Transport | Cloud messaging |
| REST | Representational State Transfer | API |
| JSON | JavaScript Object Notation | Data format |
| TWAI | Two-Wire Automotive Interface | ESP32 CAN |
| OTA | Over-The-Air | Firmware updates |
| GPIO | General Purpose Input/Output | Pins |
| SPI | Serial Peripheral Interface | Data protocol |
| I2C | Inter-Integrated Circuit | Data protocol |
| PWM | Pulse Width Modulation | Control signal |
| NVS | Non-Volatile Storage | ESP32 memory |
| JWT | JSON Web Token | Authentication |
| CORS | Cross-Origin Resource Sharing | API security |

See [GLOSSARY.md](GLOSSARY.md) for complete glossary.

## Documentation Index

Complete list of reference materials:
1. [CAN_BUS_REFERENCE.md](CAN_BUS_REFERENCE.md) - CAN protocol details
2. [MQTT_TOPICS.md](MQTT_TOPICS.md) - MQTT structure
3. [GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md) - Pin assignments
4. [HARDWARE_SPECIFICATIONS.md](HARDWARE_SPECIFICATIONS.md) - Component specs
5. [GLOSSARY.md](GLOSSARY.md) - Terms and definitions
6. [EXTERNAL_LINKS.md](EXTERNAL_LINKS.md) - Outside resources

---

See also:
- [01_Architecture/](../01_Architecture/) - System design
- [02_Hardware_Modules/](../02_Hardware_Modules/) - Module details
- [10_Reference/](.) - This section
