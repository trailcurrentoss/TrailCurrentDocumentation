# TrailCurrent Deployment Guide

Complete guide for deploying TrailCurrent systems to production and development environments.

## Overview

Deployment covers:
- Getting system ready for deployment
- Configuring infrastructure
- Deploying software components
- Managing firmware updates
- Monitoring production systems

## Deployment Tiers

### Tier 1: Hardware (Vehicles)
Individual ESP32 modules and in-vehicle compute systems deployed in vehicles.

### Tier 2: Infrastructure
Cloud services, databases, and communication services.

### Tier 3: Management
Tools and processes for managing deployed systems.

## Quick Reference

| Component | Environment | Method | Duration |
|-----------|-------------|--------|----------|
| Hardware | Vehicle | OTA Update | 5-30 min |
| Vehicle Pi | Vehicle | Docker Compose | 10-20 min |
| Cloud API | Cloud | Docker | 5-10 min |
| Frontend | Cloud | Static files | <1 min |
| Database | Cloud | Schema update | <5 min |

## Deployment Process

### Phase 1: Pre-Deployment
1. [DEPLOYMENT_CHECKLIST.md](DEPLOYMENT_CHECKLIST.md) - Verify readiness
2. Review changes and test results
3. Backup existing systems
4. Notify users if needed
5. Schedule maintenance window

### Phase 2: Deployment
Choose based on component:
- [Hardware Firmware](../02_Hardware_Modules/) - OTA process
- [Vehicle Pi](../03_Vehicle_Compute/Deployment/PI_DEPLOYMENT.md) - Docker update
- [Cloud Application](../04_Cloud_Application/Deployment/CLOUD_DEPLOYMENT.md) - Rolling update
- [Mobile App](../05_Mobile_Application/) - Play Store distribution

### Phase 3: Post-Deployment
1. Verify all systems operational
2. Monitor logs and metrics
3. Confirm user access
4. Document deployment
5. Plan rollback if needed

## Deployment Strategies

### Blue-Green Deployment
```
Current (Blue):      New (Green):
┌──────────────┐    ┌──────────────┐
│ API v1.0     │    │ API v1.1     │
│ Running      │    │ Staged       │
└──────────────┘    └──────────────┘
      ↑
  User Traffic
      │
  Switch when ready
      ↓
│ API v1.1     │
│ Active       │
└──────────────┘
```

### Rolling Deployment
```
Instance 1: v1.0 → v1.1 ✓
Instance 2: v1.0 → v1.1 ✓
Instance 3: v1.0 → v1.1 ✓
(Gradual update with health checks)
```

### Canary Deployment
```
5% traffic → v1.1  (monitor)
25% traffic → v1.1 (monitor)
50% traffic → v1.1 (monitor)
100% traffic → v1.1
```

## Hardware Module Deployment

### OTA Update Process

1. **Prepare Firmware**
   ```bash
   cd /Product/TrailCurrent[Module]
   idf.py build
   # Binary: build/[module].bin
   ```

2. **Upload to Cloud**
   - Cloud stores firmware binaries
   - Associates with device/version

3. **Distribute to Vehicles**
   - Send update notice via MQTT
   - Vehicle Pi downloads firmware
   - Pi queues update for module

4. **Module Updates**
   - Module receives update via CAN
   - Flashes new firmware
   - Reboots and validates
   - Confirms completion

5. **Verification**
   - Check module status
   - Validate functionality
   - Monitor logs

### OTA Implementation
See [FIRMWARE_UPDATES.md](FIRMWARE_UPDATES.md) for detailed OTA procedures.

## Vehicle Pi Deployment

### Initial Setup
See [03_Vehicle_Compute/Deployment/PI_DEPLOYMENT.md](../03_Vehicle_Compute/Deployment/PI_DEPLOYMENT.md)

### Container Updates
```bash
ssh pi@[ip-address]
cd /opt/trailcurrent
docker-compose pull
docker-compose up -d
```

### Configuration Changes
1. Modify config files
2. Restart relevant containers
3. Verify connectivity

## Cloud Deployment

### Development Deployment
```bash
cd /Product/TrailCurrentCloud
docker-compose -f docker-compose.dev.yml up
```

### Production Deployment
```bash
cd /Product/TrailCurrentCloud
docker-compose up -d
docker-compose logs -f
```

See [04_Cloud_Application/Deployment/CLOUD_DEPLOYMENT.md](../04_Cloud_Application/Deployment/CLOUD_DEPLOYMENT.md) for details.

## Database Migrations

### Schema Updates
```bash
# Backup first
pg_dump [database] > backup.sql

# Run migrations
psql [database] < migration.sql

# Verify
psql [database] -c "\dt"
```

### Rollback
```bash
psql [database] < rollback.sql
```

## Frontend Deployment

### Static Files
```bash
cd /Product/TrailCurrentCloud/frontend
npm run build
# Output: dist/

# Copy to web server
cp -r dist/* /var/www/html/
```

### Version Management
- Tag releases in git
- Store version in frontend code
- Display in UI footer

## Mobile App Deployment

### Google Play Store
1. Build release APK/AAB
2. Create Play Store listing
3. Upload to Google Play Console
4. Configure rollout percentage
5. Monitor crash reports

### Direct Distribution
- Host APK on website
- Distribute to users
- Users install via file manager

## Deployment Verification

### System Checks
- [ ] All services running
- [ ] Database connectivity
- [ ] API responding
- [ ] Cloud-Pi MQTT connected
- [ ] Pi-Device CAN connected

### Functional Tests
- [ ] User login works
- [ ] Device status updates
- [ ] Commands execute
- [ ] Notifications send
- [ ] Data persists

### Performance Checks
- [ ] API response time <100ms
- [ ] Page load time <2s
- [ ] WebSocket latency <200ms
- [ ] Database query time <50ms

## Monitoring & Logging

### Continuous Monitoring
```bash
# Check service status
docker ps

# View logs
docker logs [service-name]
docker logs -f [service-name]  # Follow

# System metrics
docker stats
```

### Log Aggregation (Optional)
- ELK Stack (Elasticsearch, Logstash, Kibana)
- CloudWatch (if AWS)
- Splunk (if enterprise)

## Rollback Procedures

### If Deployment Fails

**Cloud API**:
```bash
# Revert to previous image
docker-compose pull
docker-compose down
git checkout [previous-commit]
docker-compose up -d
```

**Database Migration**:
```bash
psql [database] < rollback.sql
```

**Hardware Firmware**:
```bash
# Module can rollback:
- If new firmware doesn't start
- Module reverts to previous partition
- Manually trigger rollback if needed
```

## Infrastructure Scaling

### Add More API Servers
```bash
docker-compose up -d api1 api2 api3
# Configure load balancer
```

### Database Replication
- Set up PostgreSQL streaming replication
- Configure read replicas
- Route queries to replicas

### Cache Scaling
- Redis cluster for distributed caching
- Session sharing across instances

## Security in Deployment

### Pre-Deployment
- Security review of code
- Dependency vulnerability check
- Configuration audit

### Runtime
- Monitor for unauthorized access
- Rate limiting on APIs
- Log suspicious activity
- Regular security updates

## Troubleshooting Deployments

Common deployment issues:

| Issue | Cause | Resolution |
|-------|-------|-----------|
| Service won't start | Port already in use | Check with `netstat` or `lsof` |
| Database connection fails | Wrong credentials | Verify config and restart |
| API returns 500 | Code error | Check logs with `docker logs` |
| WebSocket won't connect | Firewall blocking | Open port in firewall rules |
| Mobile app can't reach API | Wrong endpoint | Verify API URL in config |

See [09_Troubleshooting/](../09_Troubleshooting/) for more issues.

## Documentation

- [DEPLOYMENT_CHECKLIST.md](DEPLOYMENT_CHECKLIST.md) - Pre-deployment verification
- [DOCKER_SETUP.md](DOCKER_SETUP.md) - Docker configuration
- [DEPLOYMENT_SCRIPTS.md](DEPLOYMENT_SCRIPTS.md) - Automated scripts
- [FIRMWARE_UPDATES.md](FIRMWARE_UPDATES.md) - OTA update process

## Automation

### CI/CD Pipeline
Automate testing and deployment:
- GitHub Actions / GitLab CI
- Automated testing on push
- Automatic staging deployment
- Manual production approval

### Deployment Scripts
Custom scripts in `/Product/TrailCurrentDeployment/`:
- `deploy.sh` - Deploy to Pi
- `create-deployment-package.sh` - Package firmware
- `build-and-save-images.sh` - Build Docker images

## Next Steps

1. [Review deployment checklist](DEPLOYMENT_CHECKLIST.md)
2. [Choose your deployment target](README.md)
3. [Follow component-specific guides](../README.md)
4. [Monitor after deployment](README.md#monitoring--logging)

---

See also:
- [DEPLOYMENT_CHECKLIST.md](DEPLOYMENT_CHECKLIST.md) - Pre-deployment
- [DOCKER_SETUP.md](DOCKER_SETUP.md) - Docker configuration
- [FIRMWARE_UPDATES.md](FIRMWARE_UPDATES.md) - OTA updates
- [01_Architecture/DEPLOYMENT_ARCHITECTURE.md](../01_Architecture/DEPLOYMENT_ARCHITECTURE.md) - Infrastructure design
