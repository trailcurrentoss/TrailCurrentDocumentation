# TrailCurrent Troubleshooting Guide

Comprehensive troubleshooting guide for the TrailCurrent platform.

## Quick Links

- [COMMON_ISSUES.md](COMMON_ISSUES.md) - FAQ and quick solutions
- [HARDWARE_ISSUES.md](HARDWARE_ISSUES.md) - Hardware module problems
- [NETWORK_ISSUES.md](NETWORK_ISSUES.md) - Network and connectivity
- [FIRMWARE_ISSUES.md](FIRMWARE_ISSUES.md) - Firmware and OTA
- [LOGGING_AND_DIAGNOSTICS.md](LOGGING_AND_DIAGNOSTICS.md) - Debug techniques

## Troubleshooting by Component

### Hardware Modules (ESP32)

**Symptoms**:
- Module not visible on CAN bus
- Not receiving commands
- Restarting unexpectedly
- LED not responding

**Guide**: [HARDWARE_ISSUES.md](HARDWARE_ISSUES.md)

### In-Vehicle Compute System

**Symptoms**:
- Pi won't boot
- Containers failing to start
- Can't communicate with modules
- Network disconnected

**Guide**: [NETWORK_ISSUES.md](NETWORK_ISSUES.md)

### Cloud Application

**Symptoms**:
- API not responding
- WebSocket connection failing
- Database errors
- Authentication failing

**Guide**: [COMMON_ISSUES.md](COMMON_ISSUES.md)

### Mobile Application

**Symptoms**:
- Can't connect to cloud
- Commands not executing
- App crashes
- Offline data not syncing

**Guide**: [COMMON_ISSUES.md](COMMON_ISSUES.md)

## Diagnostic Approach

### Step 1: Identify the Problem
- What's not working?
- When did it start?
- What changed recently?
- What's the impact?

### Step 2: Gather Information
- Check logs
- Test connectivity
- Verify configuration
- Review recent changes

### Step 3: Isolate the Issue
- Test each component
- Check dependencies
- Verify communication
- Confirm hardware

### Step 4: Apply Solution
- Implement fix
- Test functionality
- Verify no side effects
- Document resolution

### Step 5: Follow-up
- Monitor for recurrence
- Update documentation
- Implement prevention

## Common Scenarios

### "Device is offline"

**Likely Causes**:
1. CAN bus disconnected
2. Module power supply issue
3. CAN transceiver failure
4. Software crash

**Steps**:
1. Check physical connections
2. Verify power supply
3. Check vehicle Pi logs
4. Verify CAN parameters

See [HARDWARE_ISSUES.md](HARDWARE_ISSUES.md#device-offline)

### "Command not executing"

**Likely Causes**:
1. Device not on CAN bus
2. Command permission denied
3. Device in wrong state
4. Network disconnected

**Steps**:
1. Verify device online
2. Check user permissions
3. Verify device state
4. Check command logs

See [COMMON_ISSUES.md](COMMON_ISSUES.md#command-not-executing)

### "Data not updating"

**Likely Causes**:
1. Sensor malfunction
2. CAN bus issue
3. Cloud connection down
4. MQTT broker issue

**Steps**:
1. Check sensor readings
2. Test CAN communication
3. Verify cloud connectivity
4. Check MQTT status

See [NETWORK_ISSUES.md](NETWORK_ISSUES.md#data-not-updating)

### "Cloud API error"

**Likely Causes**:
1. Service not running
2. Database down
3. Authentication issue
4. Invalid request

**Steps**:
1. Check container status
2. Verify database connectivity
3. Check API logs
4. Validate request format

See [COMMON_ISSUES.md](COMMON_ISSUES.md)

## Logging & Debugging

### Enable Debug Logging

**Hardware Modules**:
```c
// In firmware:
#define CONFIG_DEBUG_ENABLE 1
```

**Vehicle Pi**:
```bash
docker logs -f can-gateway
docker logs -f mosquitto
```

**Cloud**:
```bash
export DEBUG=trailcurrent:*
npm start
```

**Mobile**:
- Android Logcat: `adb logcat | grep trailcurrent`

See [LOGGING_AND_DIAGNOSTICS.md](LOGGING_AND_DIAGNOSTICS.md) for detailed logging setup.

## Testing Connectivity

### Test CAN Bus
```bash
# On Vehicle Pi
ip link show can0           # Check interface
cansend can0 123#1234567890 # Send test message
candump can0                # Monitor messages
```

### Test MQTT
```bash
# Subscribe to topic
mosquitto_sub -h localhost -t tc/gps/position

# Publish test
mosquitto_pub -h localhost -t tc/command/test -m '{"test": true}'
```

### Test Cloud API
```bash
curl -X GET http://localhost:3000/api/devices \
  -H "Authorization: Bearer [token]"
```

### Test Network
```bash
ping [cloud-server]
curl -I https://[cloud-server]
nc -zv [cloud-server] 443
```

## Recovery Procedures

### Pi Won't Boot

1. Check power supply
2. Verify SD card isn't corrupt
3. Try different SD card
4. Flash new OS image
5. Restore from backup

See [HARDWARE_ISSUES.md](HARDWARE_ISSUES.md#pi-wont-boot)

### Database Corrupted

1. Stop all services
2. Restore from backup
3. Run integrity check
4. Resume services

### Firmware Update Failed

1. Power cycle module
2. Module should rollback
3. Check error logs
4. Retry with previous version

See [FIRMWARE_ISSUES.md](FIRMWARE_ISSUES.md#update-failed)

## Performance Issues

### High CPU Usage
- Check for loops
- Monitor process load
- Profile application
- Optimize code

### High Memory Usage
- Check for leaks
- Monitor allocations
- Reduce buffer sizes
- Optimize data structures

### Slow Response
- Check network latency
- Profile database queries
- Check CAN bus load
- Review API efficiency

See [COMMON_ISSUES.md](COMMON_ISSUES.md#performance-issues)

## Documentation Structure

| Document | Purpose |
|----------|---------|
| [COMMON_ISSUES.md](COMMON_ISSUES.md) | FAQ and general issues |
| [HARDWARE_ISSUES.md](HARDWARE_ISSUES.md) | ESP32 module problems |
| [NETWORK_ISSUES.md](NETWORK_ISSUES.md) | Network connectivity |
| [FIRMWARE_ISSUES.md](FIRMWARE_ISSUES.md) | Firmware and OTA |
| [LOGGING_AND_DIAGNOSTICS.md](LOGGING_AND_DIAGNOSTICS.md) | Debug techniques |

## Getting Support

### Before Reporting Issue

1. Check [COMMON_ISSUES.md](COMMON_ISSUES.md)
2. Review relevant guide
3. Gather diagnostic information
4. Try documented solutions
5. Check recent changes

### When Reporting

Include:
- Component affected
- Symptoms/error messages
- Steps to reproduce
- Logs and diagnostics
- Hardware/software versions
- Recent changes

### Where to Report

- Gitea issues tracker
- Team chat/email
- Documentation updates

## Preventing Issues

### Regular Maintenance

- Monitor system health
- Review logs weekly
- Update firmware quarterly
- Test backup/restore procedures
- Document system configuration

### Best Practices

- Use stable versions
- Test before deploying
- Monitor resources
- Log important events
- Keep backups current
- Document changes

### Monitoring Setup

- Monitor disk space
- Track memory usage
- Log API errors
- Alert on high latency
- Track failed commands
- Monitor CAN errors

## Escalation

If standard troubleshooting doesn't resolve:

1. **Gather more information**
   - Enable debug logging
   - Run diagnostic tools
   - Capture network traffic

2. **Contact support**
   - Provide all logs
   - Describe issue clearly
   - Include system info

3. **Review changes**
   - What changed recently?
   - When did issue start?
   - Any config changes?

## References

- [01_Architecture/](../01_Architecture/) - System design
- [LOGGING_AND_DIAGNOSTICS.md](LOGGING_AND_DIAGNOSTICS.md) - Debug tools
- [09_Troubleshooting/COMMON_ISSUES.md](COMMON_ISSUES.md) - FAQ
- [02_Hardware_Modules/](../02_Hardware_Modules/) - Hardware reference

---

See also:
- [COMMON_ISSUES.md](COMMON_ISSUES.md) - Specific problems and solutions
- [LOGGING_AND_DIAGNOSTICS.md](LOGGING_AND_DIAGNOSTICS.md) - Debug techniques
- [07_Development/DEBUGGING.md](../07_Development/DEBUGGING.md) - Development debugging
