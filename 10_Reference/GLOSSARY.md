# TrailCurrent Glossary

Complete glossary of terms, acronyms, and definitions used in TrailCurrent documentation.

## Terms

### CAN Bus / CAN Network
Controller Area Network - A robust serial communication protocol used for real-time communication between hardware modules in TrailCurrent. All ESP32 modules communicate via CAN bus at 500 kbps or 1 Mbps.

### CAN Transceiver
Hardware component that connects an ESP32 module to the CAN bus physical layer. Common types: MCP2515 (SPI interface) or TJA1050 (direct transceiver).

### CAN ID
Unique identifier for each CAN message (11-bit standard or 29-bit extended). Used to route and filter messages on the bus.

### MQTT (Message Queuing Telemetry Transport)
Lightweight pub/sub messaging protocol used for communication between in-vehicle compute system and cloud application. Messages organized in hierarchical topics.

### Mosquitto
Open-source MQTT broker used in TrailCurrent for message brokering. Runs in Docker container on in-vehicle compute device and cloud servers.

### REST API
Representational State Transfer API - HTTP-based API for web services. TrailCurrent cloud application provides REST API for device control and data queries.

### WebSocket
Full-duplex communication protocol over TCP. Used for real-time updates from cloud to web UI and mobile apps.

### OTA (Over-The-Air)
Firmware update mechanism that distributes new code to hardware modules via CAN bus without physical connection.

### Deep Sleep
Low-power mode where ESP32 consumes minimal current (<10µA). Used for battery-powered modules to extend runtime.

### NVS (Non-Volatile Storage)
Internal flash memory on ESP32 used to store configuration, device ID, and other persistent data.

### GPIO (General Purpose Input/Output)
Digital pins on ESP32 that can be configured as input or output. Used for LED control, button sensing, relay switching.

### PWM (Pulse Width Modulation)
Technique for controlling analog devices using digital signals. Used for heater control, leveler motors, brightness control.

### I2C (Inter-Integrated Circuit)
Serial communication protocol using SDA and SCL lines. Commonly used for temperature sensors, accelerometers, etc.

### SPI (Serial Peripheral Interface)
Synchronous serial communication protocol. Used for high-speed communication with sensors and CAN transceivers.

### UART (Universal Asynchronous Receiver-Transmitter)
Serial communication interface used for GPS modules, debug output, and other serial devices.

### Relay
Electromechanical switch controlled by an electrical signal. Used in Power Control Module for switching high-power loads.

### Bootloader
Small firmware program that runs first on ESP32, initializes hardware, and loads main application.

### Firmware
Software code running on embedded devices (ESP32 modules). Typically written in C/C++ using ESP-IDF framework.

### Docker Container
Lightweight virtualization technology bundling application, dependencies, and configuration. Used for cloud services and vehicle Pi applications.

### PostgreSQL
Open-source relational database used for storing long-term data in cloud application (users, devices, sensor history).

### Redis
In-memory data store used for caching frequently accessed data and managing user sessions in cloud.

### JWT (JSON Web Token)
Authentication token format used for API requests. Contains encoded user information and signature.

### CORS (Cross-Origin Resource Sharing)
Security mechanism allowing web browsers to access APIs from different domains.

### Blue-Green Deployment
Deployment strategy with two identical production environments. One is "blue" (active), one is "green" (staging).

### Canary Deployment
Gradual rollout of new version to subset of users, then increasing percentage as validation succeeds.

### In-Vehicle Compute
Edge computing device (Raspberry Pi, Orange Pi, Jetson Nano, or similar) running services for a vehicle. Acts as gateway between hardware modules and cloud.

### Edge Computing
Computing at the network edge (on in-vehicle compute device) rather than cloud. Enables local processing and offline operation.

### Mesh Network
Network topology where nodes can relay messages through each other. TrailCurrent EspNow gateway supports mesh networking.

### CAN Termination
120Ω resistors placed at both ends of CAN bus to prevent signal reflections and improve data integrity.

### Bitrate / Baud Rate
Speed of serial communication. CAN bus typically runs at 500 kbps or 1 Mbps.

---

## Acronyms

| Acronym | Full Name | Context |
|---------|-----------|---------|
| API | Application Programming Interface | Software interface for external access |
| BT | Bluetooth | Wireless short-range communication |
| CAN | Controller Area Network | Hardware communication protocol |
| CI/CD | Continuous Integration / Continuous Deployment | Automated build and deployment |
| CORS | Cross-Origin Resource Sharing | Web security mechanism |
| CPU | Central Processing Unit | Microprocessor |
| CSV | Comma-Separated Values | Data format |
| DB | Database | Persistent data storage |
| DHCP | Dynamic Host Configuration Protocol | Network IP assignment |
| DNS | Domain Name System | Hostname to IP resolution |
| ETA | Estimated Time of Arrival | Navigation term |
| GPIO | General Purpose Input/Output | Digital pins on microcontroller |
| GPS | Global Positioning System | Satellite location tracking |
| HTTP/HTTPS | HyperText Transfer Protocol / Secure | Web protocol (secure version) |
| I2C | Inter-Integrated Circuit | Serial communication protocol |
| IP | Internet Protocol | Network protocol |
| JSON | JavaScript Object Notation | Data format |
| JWT | JSON Web Token | Authentication token |
| LED | Light Emitting Diode | Indicator light |
| MQTT | Message Queuing Telemetry Transport | Pub/sub messaging protocol |
| MCU | Microcontroller Unit | Embedded processor (ESP32) |
| NAT | Network Address Translation | Firewall/router feature |
| NVS | Non-Volatile Storage | Persistent memory on ESP32 |
| OTA | Over-The-Air | Wireless firmware update |
| PCM | Power Control Module | Central device distribution hardware |
| PNG | Portable Network Graphics | Image format |
| PWM | Pulse Width Modulation | Signal control technique |
| QoS | Quality of Service | Message delivery guarantee |
| RAM | Random Access Memory | Volatile working memory |
| REST | Representational State Transfer | API architecture style |
| RGB | Red Green Blue | Color space (LED colors) |
| RPM | Revolutions Per Minute | Motor speed |
| RTSP | Real Time Streaming Protocol | Video streaming protocol |
| RX | Receive | Serial communication direction |
| SDA | Serial Data Line | I2C protocol signal |
| SCL | Serial Clock Line | I2C protocol signal |
| SDK | Software Development Kit | Development tools and libraries |
| SPI | Serial Peripheral Interface | Synchronous serial protocol |
| SQL | Structured Query Language | Database query language |
| SSH | Secure Shell | Remote command-line access |
| SSL/TLS | Secure Sockets Layer / Transport Layer Security | Encryption protocol |
| TCP | Transmission Control Protocol | Reliable network protocol |
| TWAI | Two-Wire Automotive Interface | ESP32 built-in CAN controller |
| UDP | User Datagram Protocol | Unreliable network protocol |
| UART | Universal Asynchronous Receiver-Transmitter | Serial communication interface |
| USB | Universal Serial Bus | Computer connection standard |
| UTC | Coordinated Universal Time | Time standard |
| VPN | Virtual Private Network | Secure network tunnel |
| WiFi | Wireless Fidelity | Wireless networking standard |
| WS/WSS | WebSocket / WebSocket Secure | Real-time communication protocol |
| XML | eXtensible Markup Language | Data format |

---

## Module Abbreviations

| Abbreviation | Full Name | Type |
|--------------|-----------|------|
| GPS | GPS Module | Sensor |
| TEMP | Temperature Sensor | Sensor |
| AQI | Air Quality Module | Sensor |
| DOOR | Cabinet & Door Sensor | Sensor |
| SHUNT | Shunt Gateway | Sensor |
| PCM | Power Control Module | Control |
| HEATER | Electric Heater Control | Control |
| LEVEL | Vehicle Leveler | Control |
| MPPT | MPPT CAN Gateway | Control |
| BT | Bluetooth Gateway | Gateway |
| ESPNOW | CAN/EspNow Gateway | Gateway |
| EXT_SYS | External Systems Monitor | Gateway |
| BUTTON | 8-Button Panel | Interface |
| DISPLAY | Wall Mounted Display | Interface |
| DISPLAY_7IN | 7" Sunton Display | Interface |
| REMOTE | EspNow Remote | Interface |
| WAVESHARE | Waveshare Remote | Interface |

---

## Common Phrases

### "On the CAN bus"
Device is connected to and actively communicating via CAN bus.

### "MQTT topic"
Hierarchical message address in MQTT system (e.g., `tc/gps/position`).

### "Device offline"
Module is not responding on CAN bus, likely due to power loss or communication failure.

### "Cloud down"
Cloud services unreachable. System operates locally with cached data.

### "OTA update"
Wireless firmware update sent to module. Module updates and reboots.

### "Deep sleep"
Low-power state where module consumes minimal current. Wakes on scheduled interval or interrupt.

---

## Cross-References

For detailed information on specific topics:
- **CAN Protocol**: See [CAN_BUS_REFERENCE.md](CAN_BUS_REFERENCE.md)
- **MQTT Topics**: See [MQTT_TOPICS.md](MQTT_TOPICS.md)
- **Hardware Pins**: See [GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md)
- **Hardware Specs**: See [HARDWARE_SPECIFICATIONS.md](HARDWARE_SPECIFICATIONS.md)
- **Protocol Details**: See [01_Architecture/NETWORK_TOPOLOGY.md](../01_Architecture/NETWORK_TOPOLOGY.md)

---

See also:
- [10_Reference/](.) - Reference documentation
- [README.md](README.md) - Reference overview
