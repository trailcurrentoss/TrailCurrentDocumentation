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
- Processor recommendations (Raspberry Pi 4/5, Orange Pi, Jetson Nano)
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

### MCP2515

**NEEDS TO BE COMPLETED** - Document:
- Communication protocol (SPI)
- Clock speed support
- Voltage requirements (5V)
- Current consumption
- Operating temperature
- Termination voltage divider requirements
- Capacitor specifications
- Crystal frequency

### TJA1050

**NEEDS TO BE COMPLETED** - Document:
- Voltage requirements (5V, 3.3V variants)
- Current consumption
- Operating temperature
- Bus voltage requirements
- Slope control
- Standby mode

## Sensor Specifications

### GPS Module

**NEEDS TO BE COMPLETED** - Document:
- Position accuracy
- Velocity accuracy
- Time to first fix
- Number of satellites
- Frequency
- Power consumption
- Communication protocol
- Connector type

### Temperature Sensor

**NEEDS TO BE COMPLETED** - Document:
- Temperature range
- Accuracy
- Resolution
- Response time
- Communication protocol (1-Wire, I2C, analog)
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
