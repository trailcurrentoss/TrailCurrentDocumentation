# Hardware Specifications Reference

Complete electrical and physical specifications for all TrailCurrent hardware components.

## In-Vehicle Compute Device Requirements

### Minimum Specifications

**NEEDS TO BE COMPLETED** - Document:
- Processor requirements
- RAM minimum (current: 4GB)
- Storage minimum (current: 32GB)
- Network interface requirements
- Power consumption ranges
- Operating temperature range

### Recommended Specifications

**NEEDS TO BE COMPLETED** - Document:
- Reference hardware: Raspberry Pi Compute Module 5 (CM5) on a standard carrier board with Waveshare RS485 CAN HAT (B)
- RAM recommendations
- Storage recommendations
- Expansion options

### Power Specifications

**NEEDS TO BE COMPLETED** - Document:
- Input voltage ranges
- Power consumption (idle vs active)
- USB power requirements
- Peak current draw
- Thermal considerations

## ESP32 Module Specifications

### Processor Specifications

**NEEDS TO BE COMPLETED** - Document:
- Microcontroller model
- Clock speed
- Flash memory size
- RAM size
- GPIO count
- Peripheral availability (SPI, I2C, UART, CAN)

### Pin Specifications

**NEEDS TO BE COMPLETED** - Document:
- Voltage levels (3.3V)
- Current limits per GPIO
- Total current limit
- Input impedance
- Output impedance
- Pull-up/pull-down capabilities

### Power Specifications

**NEEDS TO BE COMPLETED** - Document:
- Input voltage range
- Current consumption (idle, active, deep sleep)
- Current limits per power rail
- Brownout detection
- Voltage regulation

### Analog Specifications

**NEEDS TO BE COMPLETED** - Document:
- ADC resolution
- ADC reference voltage
- Number of ADC channels
- Sampling rate
- Conversion time

## CAN Transceiver Specifications

TrailCurrent hardware modules use the ESP32 family's built-in TWAI (Two-Wire
Automotive Interface) controller, paired with an external CAN transceiver to
drive the physical bus. The in-vehicle compute (Headwaters) uses a separate
MCP2515 SPI CAN controller on a standard Waveshare HAT.

### SN65HVD230 (Modules)

The SN65HVD230 is the primary 3.3 V CAN transceiver used across every hardware
module — either built into Waveshare RS485-CAN / Relay carrier boards, or
wired externally on ESP32 dev boards. Key parameters:

| Parameter | Value |
|-----------|-------|
| Supply voltage | 3.3 V |
| Signaling | ISO 11898-2 compatible |
| Data rate | up to 1 Mbps (TrailCurrent uses 500 kbps) |
| Operating temperature | −40 °C to +85 °C |
| Modes | Normal, silent (listen-only), low-power standby |
| Current (normal) | ~17 mA typical |
| Bus fault protection | ±36 V |

Termination is a 120 Ω resistor at each end of the bus; intermediate modules
do not terminate. See [CAN_BUS_REFERENCE.md](./CAN_BUS_REFERENCE.md) for the
physical-layer guidelines.

### MCP2515 (Headwaters Only)

The Raspberry Pi CM5 does not have a built-in CAN controller, so Headwaters
uses the **Waveshare RS485 CAN HAT (B)** — an off-the-shelf HAT that presents
an MCP2515 SPI CAN controller plus a standard transceiver to the Pi. No
custom PCB or soldering is required.

| Parameter | Value |
|-----------|-------|
| Interface to host | SPI (`/dev/spidev0.0`) |
| CAN controller | Microchip MCP2515 |
| Clock | 8 MHz crystal (configured in the Linux device tree overlay) |
| Max data rate | 1 Mbps (TrailCurrent uses 500 kbps) |
| Bus fault protection | Handled by the transceiver chip on the HAT |

Linux exposes the controller as a SocketCAN interface (`/dev/can0` /
`can0`). The Headwaters backend consumes frames through SocketCAN without
knowing or caring that it is backed by an MCP2515.

## Sensor Specifications

### Bearing (GNSS Module)

**NEEDS TO BE COMPLETED** - Document:
- Position accuracy
- Velocity accuracy
- Time to first fix
- Number of satellites
- Frequency
- Power consumption
- Communication protocol
- Connector type

### Borealis (Environment Module)

**NEEDS TO BE COMPLETED** - Document:
- Temperature range and accuracy
- Humidity range and accuracy
- CO2 measurement range
- Resolution
- Response time
- Communication protocol (I2C)
- Connector type
- Environmental ratings

### Current Shunt Monitor

**NEEDS TO BE COMPLETED** - Document:
- Shunt resistance value
- Maximum current rating
- Accuracy
- Temperature coefficient
- Power dissipation
- Connector type

## Relay Specifications

### Power Relays (Heater, Fans, etc.)

**NEEDS TO BE COMPLETED** - Document:
- Coil voltage
- Coil current
- Contact rating (voltage/current)
- Switching frequency
- Lifespan
- Switching time
- Type (Normally Open, Normally Closed, etc.)

## Connector Specifications

**NEEDS TO BE COMPLETED** - Document:
- Connector types used (M12, DT, etc.)
- Pin assignments
- Voltage ratings
- Current ratings
- Environmental sealing
- Mating cycles

## Environmental Specifications

**NEEDS TO BE COMPLETED** - Document:
- Operating temperature range
- Storage temperature range
- Humidity range
- IP rating (if applicable)
- Vibration tolerance
- Altitude limits
- Salt spray resistance (RV/Marine)

## Mechanical Specifications

**NEEDS TO BE COMPLETED** - Document:
- Enclosure dimensions
- Weight
- Mounting options
- Cable routing
- Strain relief
- IP rating

## Power Distribution Specifications

**NEEDS TO BE COMPLETED** - Document:
- Input voltage ranges
- Fused circuits
- Protection devices
- Voltage rails available
- Current capacity per rail
- Decoupling capacitor requirements

## Data Rate Specifications

**NEEDS TO BE COMPLETED** - Document:
- CAN bus bitrate (500k, 1M)
- UART speeds
- I2C speeds
- SPI speeds
- Network speeds (Ethernet, WiFi if used)

## Compliance & Standards

**NEEDS TO BE COMPLETED** - Document:
- RoHS compliance
- FCC/CE certifications
- UL ratings
- Safety standards
- EMC specifications

## Derating Curves

**NEEDS TO BE COMPLETED** - Provide:
- Thermal derating
- Voltage derating
- Current derating
- Frequency derating

---

## Related Documentation

- [GPIO_PIN_MAPPING.md](GPIO_PIN_MAPPING.md) - Pin assignments
- [GLOSSARY.md](GLOSSARY.md) - Technical terminology
- [../02_Hardware_Modules/](../02_Hardware_Modules/) - Module-specific specs
