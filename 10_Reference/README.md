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
0x000-0x0FF: Bearing (GNSS)         (tc/gps/*)
0x100-0x1FF: Borealis (Environment)(tc/temp/*)
0x200-0x2FF: Torrent (Power)       (tc/power/*)
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

### Torrent (Power Delivery Module)
- **CAN ID**: 0x200 (commands), 0x280 (status)
- **Outputs**: 8 switched/dimmed power channels
- **Inputs**: Status from other modules
- **Key Pins**: GPIO for MOSFETs/relays, CAN pins

### Bearing (GNSS Module)
- **CAN ID**: 0x100-0x10F
- **Interface**: Serial UART to GNSS module
- **Frequency**: 1 Hz typical
- **Accuracy**: 5m typical (depends on receiver)

### Borealis (Environment Module)
- **CAN ID**: 0x120-0x12F
- **Interfaces**: I2C
- **Sensors**: Temperature, humidity, CO2
- **Range**: -40°C to +125°C typical (temperature)

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

See [CAN_BUS_REFERENCE.md](CAN_BUS_REFERENCE.md) for the authoritative per-message table sourced from [TrailCurrent.dbc](../TrailCurrent.dbc). At-a-glance allocation:

| Range | Used for | Notes |
|---|---|---|
| `0x00 – 0x42` | All ESP32-class hardware modules | Bearing, Picket, Torrent, Borealis, Solstice, Switchback, Plateau, Reservoir, Therma, plus Headwaters-originated discovery/OTA/provisioning frames. Next available: `0x43`. |
| `0x100 – 0x12F` | Playbill head-unit | Three contiguous 16-ID blocks (`0x100/0x110/0x120`), one per Playbill instance. See [CAN_BUS_REFERENCE.md — Playbill multi-instance block](CAN_BUS_REFERENCE.md#playbill-multi-instance-block). |

### Port Numbers

| Service | Port | Protocol | Where |
|---------|------|----------|-------|
| REST API | 3000 | HTTP (dev) | Headwaters local |
| REST API | 443 | HTTPS (prod) | Farwatch cloud |
| MQTT | 1883 | TCP | Headwaters local broker (in-container only) |
| MQTT TLS | 8883 | TCP+TLS | Vehicle ↔ cloud and in-vehicle clients |
| WebSocket | 8080 | WS (dev) | Headwaters local |
| WebSocket | 443 | WSS (prod, same port as REST) | Farwatch cloud |
| MongoDB | 27017 | TCP | Internal to Docker network |
| tileserver-gl | 8080 | HTTP | Internal |
| **NTP** | **123** | **UDP** | **Headwaters serves to all Linux-class devices on the rig (Peregrine, Playbill). See [NETWORK_TOPOLOGY.md — Time Synchronization](../01_Architecture/NETWORK_TOPOLOGY.md#time-synchronization).** |
| Peregrine Web Chat | 443 | HTTPS | `https://peregrine.local/` — LAN-facing chat UI exposing the on-device LLM. Port 80 redirects to 443 and serves `/ca.pem` unencrypted for first-time CA install. |

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
