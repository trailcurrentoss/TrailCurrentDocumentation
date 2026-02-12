# GPIO Pin Mapping Reference

Complete reference for GPIO pin assignments, connections, and configurations across all hardware modules.

## In-Vehicle Compute Device (Raspberry Pi/Orange Pi/etc.)

### Raspberry Pi GPIO Header

**NEEDS TO BE COMPLETED** - Document:
- GPIO pin numbers and functions
- SPI pins (MOSI, MISO, SCK, CE)
- I2C pins (SDA, SCL)
- UART pins (RX, TX)
- CAN transceiver connections
- Reserved pins
- Pin voltage levels (3.3V vs 5V)
- Internal pull-ups/pull-downs

### Raspberry Pi SPI Configuration

**NEEDS TO BE COMPLETED** - Document:
- SPI0 pins for CAN transceiver
- SPI1 pins (optional expansion)
- CAN transceiver chip select
- Clock speed configuration
- Wiring diagram

### Raspberry Pi I2C Configuration

**NEEDS TO BE COMPLETED** - Document:
- I2C-1 pins
- Pullup resistor values
- Device addresses
- Connected devices

### Raspberry Pi UART Configuration

**NEEDS TO BE COMPLETED** - Document:
- UART0 (serial console)
- UART1-UART5 available
- Baud rate settings
- GPS module connection
- Debug serial connection

## ESP32 Module GPIO Assignments

### Standard Module Template

**NEEDS TO BE COMPLETED** - Document for each module type:
- LED pins and status colors
- Button/switch inputs
- Relay outputs
- Sensor inputs (analog/digital)
- Communication pins (CAN, I2C, SPI, UART)
- Power pins and current limits

### GPS Module (GPS_Module.md expected)

**NEEDS TO BE COMPLETED** - Reference diagram and pin list

### Temperature Sensor (TEMP_Sensor.md expected)

**NEEDS TO BE COMPLETED** - Reference diagram and pin list

### Power Control Module (PCM.md expected)

**NEEDS TO BE COMPLETED** - Reference diagram and pin list

### Other Modules

**NEEDS TO BE COMPLETED** - Pin assignments for:
- Heater Control Module
- Leveler Control Module
- Door/Cabinet Sensor
- Shunt Monitor
- MPPT Gateway
- Bluetooth Gateway
- EspNow Gateway
- Button Panel
- Display Modules

## CAN Transceiver Wiring

### MCP2515 (SPI-based)

**NEEDS TO BE COMPLETED** - Document:
- SPI pin connections (MOSI, MISO, SCK, CS)
- INT pin connection
- Crystal specifications
- Termination resistor placement
- Schematic diagram

### TJA1050 (Direct CAN)

**NEEDS TO BE COMPLETED** - Document:
- GPIO to CAN pins mapping
- Termination resistor placement
- Biasing resistors
- Power supply requirements
- Schematic diagram

## Power Distribution

**NEEDS TO BE COMPLETED** - Document:
- 5V rail assignments
- 3.3V rail assignments
- Ground connections
- Current limits per pin
- Protection circuits

## I2C Device Addresses

**NEEDS TO BE COMPLETED** - Reference table:
- Temperature sensors
- Accelerometers
- Other I2C devices
- Address conflicts
- Pullup requirements

## Pin Usage Summary

**NEEDS TO BE COMPLETED** - Provide summary tables:
- Pins used vs available
- Function assignments
- Availability for expansion
- Reserved pins
- Conflict detection

## Schematic Diagrams

**NEEDS TO BE COMPLETED** - Include or reference:
- Raspberry Pi breakout diagram
- CAN transceiver wiring
- Power distribution
- Full system schematic
- Module expansion diagrams

## LED Status Indicators

**NEEDS TO BE COMPLETED** - Document LED meanings:
- Power indicator
- Status/activity LED
- Error/warning LED
- Module-specific indicators
- Color coding (if applicable)
- Blink patterns

## Configuration Files

**NEEDS TO BE COMPLETED** - Document:
- Device tree overlays (for Raspberry Pi)
- Configuration files
- How to enable/disable interfaces
- Troubleshooting pin conflicts

## Tools & Testing

**NEEDS TO BE COMPLETED** - Document:
- GPIO testing tools
- Pin voltage measurement
- I2C/SPI scanning
- Continuity testing
- Logic analyzer usage

---

## Related Documentation

- [HARDWARE_SPECIFICATIONS.md](HARDWARE_SPECIFICATIONS.md) - Electrical specifications
- [GLOSSARY.md](GLOSSARY.md) - GPIO and pin terminology
- [CAN_BUS_REFERENCE.md](CAN_BUS_REFERENCE.md) - CAN wiring details
- [../02_Hardware_Modules/](../02_Hardware_Modules/) - Module-specific pinouts
