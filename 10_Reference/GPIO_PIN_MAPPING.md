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

### Bearing (GNSS Module)

**NEEDS TO BE COMPLETED** - Reference diagram and pin list

### Borealis (Environment Module)

**NEEDS TO BE COMPLETED** - Reference diagram and pin list

### Torrent (Power Delivery Module)

**NEEDS TO BE COMPLETED** - Reference diagram and pin list

### Other Modules

**NEEDS TO BE COMPLETED** - Pin assignments for:
- Therma (climate relay controller)
- Plateau (vehicle level sensor)
- Picket (cabinet & door sensors)
- Ampline (shunt interface)
- Solstice (MPPT solar controller)
- Switchback (6-channel relay module)
- Aftline (trailer wiring monitor)
- Tapper (8-button panel)
- Fireside (wireless touchscreen display)
- Milepost (hardwired CAN bus touchscreen)
- Spotter (trailer monitor display)

## CAN Transceiver Wiring

### SN65HVD230 (ESP32 Modules)

All ESP32 hardware modules use the built-in TWAI (Two-Wire Automotive Interface) controller paired with an external SN65HVD230 CAN transceiver. The TWAI controller handles protocol and timing; the SN65HVD230 converts logic-level signals to CAN bus differential voltages.

**Connections:**
- **TWAI TX** (ESP32 GPIO) → SN65HVD230 **D** (driver input)
- **TWAI RX** (ESP32 GPIO) → SN65HVD230 **R** (receiver output)
- **SN65HVD230 CANH/CANL** → CAN bus wiring
- **VCC**: 3.3V
- **RS** (slope control): Tie to GND for high-speed mode

TX and RX GPIO pins vary per module — defined in each module's `src/globals.h`.

**NEEDS TO BE COMPLETED** - Document specific TX/RX pins per module.

### MCP2515 (Raspberry Pi / Headwaters Only)

The Raspberry Pi does not have a built-in CAN controller, so Headwaters uses an MCP2515 SPI-based CAN controller on a HAT/shield.

**NEEDS TO BE COMPLETED** - Document:
- SPI pin connections (MOSI, MISO, SCK, CS)
- INT pin connection
- Crystal specifications
- Termination resistor placement
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
