# Firmware Update Guide

Comprehensive guide to distributing and managing firmware updates for TrailCurrent modules.

## Overview

TrailCurrent runs two distinct firmware-update paths in parallel, because the platform has two distinct classes of device. Choose the path that matches the device, not the other way around.

| Device class | Discovery path | Update transport | Examples |
|---|---|---|---|
| **CAN-attached MCU modules** | Headwaters broadcasts `OtaUpdateNotification` (`0x00`) on CAN; the targeted module joins WiFi and pulls the binary | WiFi (HTTPS to Headwaters) | Bearing, Borealis, Torrent, Switchback, Picket, Plateau, Reservoir, Therma, Solstice, Aftline, Tapper, Milepost, Fireside, Spotter |
| **Wireless modules (no CAN attachment)** | mDNS browse — Headwaters runs a `discovery-mdns.py` browser that finds and onboards LAN-attached modules without requiring them to ever appear on the CAN bus | WiFi (HTTPS to Headwaters) | **Fireside** is the first wireless module on this pattern; the pattern is intentionally generic and will be reused for future wireless devices. |
| **Linux-class compute devices** | Image rebuild + reflash for OS/dependency/model changes; `deploy.sh` for source-only iteration | edl-ng SPI NOR + NVMe flash, or rsync over SSH | Peregrine, Playbill, Headwaters Q6A variant |

The two MCU paths share the same WiFi-side OTA mechanics — they differ only in how Headwaters **finds** the module and decides to push an update.

## Wireless Module Discovery

Wireless modules don't sit on the CAN bus, so `DiscoveryTrigger` (`0x02`) and `DiscoveryReset` (`0x03`) don't reach them. Instead, Headwaters runs an mDNS browser (`local_code/discovery-mdns.py`) that walks the LAN looking for advertised TrailCurrent services, then drives onboarding entirely over MQTT and HTTP.

### Topic surface

| MQTT topic | Direction | Purpose |
|---|---|---|
| `discovery/browse/start` | PWA → discovery service | Begin a browse window (firmware advertises for 30 s) |
| `discovery/browse/stop` | PWA → discovery service | End the browse window early |
| `discovery/browse/found` | discovery service → PWA | Each device found during the browse window |
| `discovery/confirm/request` | PWA → discovery service | MCU-style flow: ask the module to flash an LED / play a tone to physically identify it |
| `discovery/confirm/response` | discovery service → PWA | Result of the confirm round-trip |
| `discovery/claim/request` | PWA → discovery service | Linux-style flow: claim the module with credentials |
| `discovery/claim/response` | discovery service → PWA | Result of the claim |

### Two onboarding patterns

The discovery service supports two patterns depending on what the module can do:

1. **MCU pattern (`confirm`)** — the module exposes a plain HTTP GET `/discovery/confirm` endpoint with no auth. The browse → confirm → adopt sequence is appropriate for small MCU-class devices where there's nothing useful to authenticate against yet.
2. **Linux pattern (`claim`)** — the module exposes HTTP POST `/discovery/claim` and accepts credentials. Used for Linux-class devices (Peregrine being the obvious next consumer) where the device has a real user account and TLS surface.

### Why this is separate from the CAN OTA path

Adding `Fireside` to the platform forced this split. Fireside is a battery-powered wireless touchscreen — it has no CAN wiring and never participates on the bus. The existing CAN-broadcast OTA path could not reach it. Rather than retrofit a CAN attachment, the wireless discovery path was added as a sibling to the CAN path; the same MQTT-driven OTA mechanism then carries the update payload once the module has been adopted.

This is intentionally a **generic pattern**, not a Fireside-specific hack. Future wireless modules onboard the same way with no additional Headwaters code.

### Where to look

- Headwaters mDNS browser: `/Product/TrailCurrentHeadwaters/local_code/discovery-mdns.py`
- Headwaters mDNS resolver service: `containers/backend/src/services/mdns-resolver.js`
- Headwaters discovery REST route: `containers/backend/src/routes/discovery.js`
- PWA discovery flow: `containers/frontend/public/js/pages/...`

## Deployment Package Distribution (Zip via PWA)

The Headwaters PWA accepts a zipped deployment package directly through the Overlook UI — the same artifact the cloud OTA watcher would normally fetch from Farwatch, just uploaded locally instead. Useful for:

- Bringing a freshly-built deployment up over a USB stick when the rig has no internet
- Side-loading a release candidate before pushing to the cloud
- Recovering an offline rig

The cloud OTA watcher (in `OTA_DEPLOYMENT_IMPLEMENTATION.md`) is still the canonical production path; the PWA upload is the offline / local-loop variant. Both apply the same package format.

## Update Architecture

### Update Flow Diagram

**NEEDS TO BE COMPLETED** - Document:
- Cloud stores firmware
- Pi downloads firmware
- Pi distributes to modules
- Modules update independently
- Rollback mechanisms

### Safety Mechanisms

**NEEDS TO BE COMPLETED** - Document:
- Signature verification
- Checksum validation
- Rollback partitions
- Watchdog protection
- Power loss recovery

## Preparing Firmware Updates

### Building Firmware

**NEEDS TO BE COMPLETED** - Steps:
1. Make code changes
2. Build firmware (idf.py build)
3. Extract binary
4. Generate checksums
5. Create update manifest
6. Version numbering

### Testing Firmware

**NEEDS TO BE COMPLETED** - Procedures:
- Build validation
- Size validation
- Functionality testing
- OTA packaging test
- Update simulation
- Rollback testing

### Packaging Firmware

**NEEDS TO BE COMPLETED** - Document:
- Binary file preparation
- Manifest creation
- Checksum generation
- Digital signing
- Package versioning
- Release notes

## Distributing Firmware Updates

### Cloud Storage

**NEEDS TO BE COMPLETED** - Setup:
- Firmware storage location
- Version management
- Access control
- Backup procedures
- Archive policies

### Update Announcement

**NEEDS TO BE COMPLETED** - Document:
- Sending update notice via MQTT
- Update availability checking
- Version information
- Release notes distribution
- User notification

### Update Scheduling

**NEEDS TO BE COMPLETED** - Procedures:
- Safe update windows
- User preferences
- Vehicle activity status
- Power status
- Network connectivity

## Update Process

### Module Update Procedure

**NEEDS TO BE COMPLETED** - Steps:
1. Module receives update notification
2. Module checks update availability
3. Module downloads firmware
4. Module verifies checksums
5. Module flashes new firmware
6. Module validates boot
7. Module confirms update success
8. Module sends confirmation to cloud

### Multi-Module Updates

**NEEDS TO BE COMPLETED** - Procedures:
- Update sequencing
- Dependency handling
- Rollback coordination
- Failure isolation
- Progress tracking

### In-Vehicle Compute Updates

**NEEDS TO BE COMPLETED** - Procedures:
- Docker container updates
- Service restart procedures
- Zero-downtime updates
- Fallback mechanisms
- Data preservation

## Monitoring Updates

### Update Status

**NEEDS TO BE COMPLETED** - Track:
- Update initiation
- Download progress
- Verification status
- Flash progress
- Reboot status
- Post-update verification

### Error Handling

**NEEDS TO BE COMPLETED** - Handle:
- Download failures
- Checksum mismatch
- Flash errors
- Boot failures
- Rollback triggers
- Error reporting

### Logging

**NEEDS TO BE COMPLETED** - Document:
- Update logs
- Flash logs
- Boot logs
- Error logs
- Rollback logs
- Timestamps

## Rollback Procedures

### Automatic Rollback

**NEEDS TO BE COMPLETED** - Conditions:
- Watchdog timeout
- Boot failures
- Critical errors
- Health check failures
- Manual trigger

### Manual Rollback

**NEEDS TO BE COMPLETED** - Procedure:
- Command sending
- Rollback execution
- Verification
- Status confirmation

### Rollback Validation

**NEEDS TO BE COMPLETED** - Verify:
- Previous version active
- Functionality restored
- No data loss
- Clean boot
- Performance normal

## Version Management

### Version Numbering

**NEEDS TO BE COMPLETED** - Scheme:
- Major.Minor.Patch format
- Build metadata
- Pre-release versions
- Compatibility indicators
- Legacy version support

### Version Checking

**NEEDS TO BE COMPLETED** - Procedures:
- Current version detection
- Update availability check
- Compatibility validation
- Dependency verification
- Prerequisite checks

### Version History

**NEEDS TO BE COMPLETED** - Track:
- Installed versions
- Update history
- Rollback history
- Known issues per version
- Support lifecycle

## Security

### Firmware Signing

**NEEDS TO BE COMPLETED** - Procedures:
- Key management
- Signature generation
- Signature verification
- Certificate validation
- Key rotation

### Update Authentication

**NEEDS TO BE COMPLETED** - Verify:
- Cloud authentication
- Module authentication
- Secure communication
- Replay attack prevention
- Man-in-the-middle prevention

### Data Protection

**NEEDS TO BE COMPLETED** - Ensure:
- Firmware encryption in transit
- Data preservation during update
- Configuration preservation
- Secure storage
- Audit logging

## Testing Updates

### Lab Testing

**NEEDS TO BE COMPLETED** - Procedures:
- Full system testing
- Rollback testing
- Failure scenario testing
- Load testing
- Reliability testing

### Staged Rollout

**NEEDS TO BE COMPLETED** - Strategy:
- Internal testing phase
- Early adopter phase
- Limited rollout
- Full rollout
- Monitoring at each stage

### Canary Updates

**NEEDS TO BE COMPLETED** - Procedure:
- Deploy to small percentage
- Monitor for issues
- Increase percentage
- Full deployment
- Rollback if needed

## User Communication

### Release Notes

**NEEDS TO BE COMPLETED** - Include:
- What's new
- Bug fixes
- Known issues
- Breaking changes
- Migration guide
- Timeline

### Update Instructions

**NEEDS TO BE COMPLETED** - Provide:
- When to update
- How to update
- What to expect
- Estimated duration
- Rollback procedure

### Support Information

**NEEDS TO BE COMPLETED** - Document:
- Help channels
- Known issues
- Troubleshooting
- Contact information
- FAQ

## Monitoring Production Updates

**NEEDS TO BE COMPLETED** - Track:
- Adoption rate
- Success rate
- Failure rate
- Error patterns
- Performance impact
- User reports

## Tools and Utilities

**NEEDS TO BE COMPLETED** - Document:
- Firmware builder
- OTA packager
- Update distribution tool
- Update monitor
- Rollback utility
- Version checker

## Troubleshooting Updates

**NEEDS TO BE COMPLETED** - Guide for:
- Update failures
- Checksum mismatches
- Flash errors
- Boot failures
- Rollback issues

---

## Related Documentation

- [README.md](README.md) - Deployment overview
- [DEPLOYMENT_CHECKLIST.md](DEPLOYMENT_CHECKLIST.md) - Pre-deployment checklist
- [../09_Troubleshooting/FIRMWARE_ISSUES.md](../09_Troubleshooting/FIRMWARE_ISSUES.md) - Firmware troubleshooting
- [../02_Hardware_Modules/](../02_Hardware_Modules/) - Module details
