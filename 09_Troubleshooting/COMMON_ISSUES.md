# Common Issues & Solutions

Common problems and their solutions organized by component.

## Hardware Issues

### CAN Bus Problems

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Module not visible on CAN bus
- CAN message errors
- Timeout errors
- Bus off errors
- Arbitration errors
- Collision detection

### GPIO/Pin Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- GPIO not responding
- Voltage measurement incorrect
- I2C/SPI communication failure
- Pin conflicts
- Pullup/pulldown issues

### Sensor Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Sensor not responding
- Incorrect readings
- Intermittent failures
- Calibration problems
- Environmental effects

### Power Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Module won't power on
- Intermittent power loss
- Current draw too high
- Voltage drops
- Protection circuit triggering

## Software Issues

### Firmware Build Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Build errors
- Compilation failures
- Missing dependencies
- Configuration conflicts
- Version mismatches

### Firmware Runtime Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Module crashes/reboots
- Watchdog timeout
- Memory corruption
- Stack overflow
- Heap exhaustion

### OTA Update Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Update fails to start
- Update corrupts
- Rollback errors
- Version mismatch
- Network timeout during update

## Network Issues

### MQTT Connectivity

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Broker unreachable
- Authentication failure
- Topic subscription issues
- Message delivery failure
- QoS problems

### WiFi/Ethernet Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Connection drops
- DNS resolution failure
- IP address issues
- DHCP problems
- Network timeouts

### Cloud Connectivity

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Cloud unreachable
- SSL/TLS errors
- Authentication timeout
- Data sync failure
- Offline mode issues

## SMS Notification Issues

### SMS not received but backend logs say "SMS sent"

The backend reports success when the SSH command exits 0. However, `sendsms` on the
GL-iNet router always exits 0 -- it only queues the message. The `smsd` daemon sends
it later and may fail silently.

**Diagnose:** SSH into the router and check for failed messages:

```bash
ssh root@192.168.8.1 'ls /etc/spool/sms/failed/*/'
```

If files exist, read one to see the failure reason:

```bash
ssh root@192.168.8.1 'cat /etc/spool/sms/failed/2-1/send_XXXXXX'
```

**Common cause:** Check the `Fail_reason` line in the failed message file. Possible
causes include SIM card not supporting SMS, no cellular signal, or modem device path
changed.

**Fix:** Clear the failed spool and re-test:

```bash
ssh root@192.168.8.1 'rm /etc/spool/sms/failed/2-1/send_*'
```

### No `[Alarm]` log lines visible

Alarm logs get buried under high-frequency GPS/sensor data. Filter them:

```bash
docker compose logs backend 2>&1 | grep -i alarm | tail -20
```

If there are zero alarm lines even at startup, check that the CAN bridge initialized:

```bash
docker compose logs backend 2>&1 | grep 'CAN Bridge'
```

### Too many SMS messages

Reduce **Max messages** on the Settings page (SMS Notifications section). The default
is 3 messages per 60 minutes. Note that an "All Off" command can trigger up to 8
relay state changes, each generating an alarm event.

### Sent message files accumulating on router

The `smsd` daemon does not auto-clean `/etc/spool/sms/sent/`. Periodically remove
old files:

```bash
ssh root@192.168.8.1 'rm /etc/spool/sms/sent/2-1/send_*'
```

See [03_Vehicle_Compute/SMS_NOTIFICATIONS.md](../03_Vehicle_Compute/SMS_NOTIFICATIONS.md)
for full documentation.

## Performance Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- High latency
- Message loss
- CPU usage high
- Memory usage high
- Storage full
- Network congestion

## Configuration Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Configuration file errors
- Settings not applying
- Default values not working
- Environment variable issues
- Path resolution errors

## Integration Issues

**NEEDS TO BE COMPLETED** - Troubleshooting for:
- Modules not communicating
- Data mismatch
- Timing synchronization
- Version incompatibility
- Protocol mismatches

## Diagnostic Tools

**NEEDS TO BE COMPLETED** - Document:
- How to enable debug logging
- Available debug modes
- Serial console access
- Network packet capture
- System log analysis
- Hardware testing tools

## Getting Help

**NEEDS TO BE COMPLETED** - Document:
- How to capture debug logs
- What information to include in bug reports
- Community support channels
- Issue reporting templates
- Escalation procedures

---

## Related Documentation

- [HARDWARE_ISSUES.md](HARDWARE_ISSUES.md) - Detailed hardware troubleshooting
- [NETWORK_ISSUES.md](NETWORK_ISSUES.md) - Network and connectivity issues
- [FIRMWARE_ISSUES.md](FIRMWARE_ISSUES.md) - Firmware-specific issues
- [LOGGING_AND_DIAGNOSTICS.md](LOGGING_AND_DIAGNOSTICS.md) - Diagnostic procedures
- [README.md](README.md) - Troubleshooting overview
