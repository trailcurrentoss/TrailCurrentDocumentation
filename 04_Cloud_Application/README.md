# TrailCurrent Cloud Application

Complete guide to the TrailCurrent cloud platform for monitoring, control, and analytics.

## Overview

The Cloud Application provides:
- REST API for data access and device control
- Web dashboard for real-time monitoring
- Mobile app backend support
- Historical data storage and analytics
- Real-time WebSocket updates
- User authentication and permissions

## Architecture

```
┌─────────────────────────────────────────────┐
│         Cloud Infrastructure                │
├─────────────────────────────────────────────┤
│                                             │
│  Frontend Layer:                            │
│  ├─ Web UI (HTML/CSS/JavaScript)           │
│  ├─ Mobile App (REST API)                  │
│  └─ WebSocket (Real-time updates)          │
│         ↓ HTTPS/WebSocket                  │
│  ┌─────────────────────────────────────┐   │
│  │  REST API Server (Node.js/Express) │   │
│  ├─────────────────────────────────────┤   │
│  │ • Authentication                   │   │
│  │ • Device management                │   │
│  │ • Data queries                     │   │
│  │ • Command routing                  │   │
│  └────────┬────────────────────────────┘   │
│           │                                 │
│  ┌────────┴──────────────────────────────┐ │
│  │  Data Services                       │ │
│  ├──────────────────────────────────────┤ │
│  │ • PostgreSQL (persistent data)      │ │
│  │ • Redis (cache & sessions)          │ │
│  │ • MQTT Broker (messaging)           │ │
│  │ • File Storage (firmware, logs)     │ │
│  └──────────────────────────────────────┘ │
│           ↑                                │
│  ┌────────────────────────────────────────┐│
│  │  MQTT Gateway to Vehicle              ││
│  │  (Connects to vehicle Pi)              ││
│  └────────────────────────────────────────┘│
│                                             │
└─────────────────────────────────────────────┘
```

## Technology Stack

| Layer | Technology | Purpose |
|-------|-----------|---------|
| Frontend | HTML/CSS/JavaScript + Bootstrap | Web UI |
| Backend | Node.js + Express | REST API |
| Database | PostgreSQL | Persistent storage |
| Cache | Redis | Session & data cache |
| Messaging | Mosquitto MQTT | Cloud-to-vehicle communication |
| Hosting | Docker | Containerization |

## Key Components

### 1. REST API Server
- User authentication & authorization
- Device CRUD operations
- Data queries and filtering
- Command routing to vehicles
- WebSocket upgrade handling

### 2. Web Frontend
- Dashboard with real-time updates
- Device management UI
- Settings and configuration
- User profile management

### 3. Backend Database
- User accounts and permissions
- Device registry and metadata
- Historical sensor data
- Command history
- System events and logs

### 4. MQTT Integration
- Real-time messaging
- Device status updates
- Command queuing
- Bi-directional vehicle communication

## Setup & Installation

### Prerequisites
- Node.js 24+
- PostgreSQL 12+
- Redis (optional but recommended)
- Docker and Docker Compose
- Internet connectivity

### Quick Start

1. [Clone Repository](SETUP_GUIDE.md#step-1-clone-repository)
2. [Install Dependencies](SETUP_GUIDE.md#step-2-install-dependencies)
3. [Configure Environment](SETUP_GUIDE.md#step-3-configure-environment)
4. [Initialize Database](SETUP_GUIDE.md#step-4-initialize-database)
5. [Run Application](SETUP_GUIDE.md#step-5-run-application)

See [SETUP_GUIDE.md](SETUP_GUIDE.md) for detailed instructions.

## Backend

### REST API Endpoints

**Authentication**
- `POST /api/auth/login` - User login
- `POST /api/auth/logout` - User logout
- `POST /api/auth/refresh` - Refresh token

**Devices**
- `GET /api/devices` - List devices
- `GET /api/devices/:id` - Get device details
- `POST /api/devices` - Register new device
- `PUT /api/devices/:id` - Update device
- `DELETE /api/devices/:id` - Remove device

**Sensor Data**
- `GET /api/data/:deviceId` - Get latest readings
- `GET /api/data/:deviceId/history` - Get historical data
- `GET /api/data/:deviceId/stats` - Get statistics

**Commands**
- `POST /api/commands` - Issue command to device
- `GET /api/commands/:id` - Get command status
- `DELETE /api/commands/:id` - Cancel command

See [Backend/API_REFERENCE.md](Backend/API_REFERENCE.md) for complete API documentation.

### Database Schema

Tables include:
- `users` - User accounts
- `devices` - Registered devices
- `sensor_readings` - Historical data
- `commands` - Command history
- `events` - System events
- `permissions` - Access control

See [Backend/DATABASE_SCHEMA.md](Backend/DATABASE_SCHEMA.md) for full schema.

### Authentication

Uses JWT (JSON Web Tokens) for stateless authentication:
- Login generates access token (short-lived)
- Refresh token for obtaining new access tokens
- Role-based access control (RBAC)

See [Backend/AUTHENTICATION.md](Backend/AUTHENTICATION.md) for details.

## Frontend

### Dashboard Features
- Real-time device status
- Sensor data visualization
- Map display (for GPS data)
- Command history
- Alerts and notifications

### Device Control
- Power controls
- Heater control
- Leveler control
- Command queueing

### Administration
- User management
- Device registry
- Firmware management
- System settings

See [Frontend/FEATURES.md](Frontend/FEATURES.md) for full feature list.

## Deployment

### Development
```bash
docker-compose -f docker-compose.dev.yml up
```
See [Deployment/DOCKER_COMPOSE.md](Deployment/DOCKER_COMPOSE.md)

### Production
```bash
docker-compose up -d
```
See [Deployment/CLOUD_DEPLOYMENT.md](Deployment/CLOUD_DEPLOYMENT.md)

## Configuration

### Environment Variables
- `NODE_ENV` - Development/production
- `DATABASE_URL` - PostgreSQL connection
- `REDIS_URL` - Redis connection
- `JWT_SECRET` - Token signing key
- `MQTT_URL` - Vehicle MQTT broker

See [SETUP_GUIDE.md](SETUP_GUIDE.md) for configuration details.

## Security

### API Security
- HTTPS only (TLS/SSL)
- CORS configured for authorized origins
- Rate limiting on endpoints
- Input validation and sanitization
- SQL injection prevention

### Database Security
- Encrypted connections
- Regular backups
- Access controls
- Audit logging

### Credential Management
- Secrets in environment variables
- Never committed to repository
- Rotation policies

## Real-Time Updates

WebSocket connection provides:
- Device status changes
- New sensor readings
- Command completions
- Alert notifications

## Monitoring & Logging

### Application Logs
- Request logging (Express)
- Error tracking
- Performance metrics

### Database Monitoring
- Query performance
- Connection pooling
- Backup verification

### System Health
- CPU and memory usage
- Disk space
- Network connectivity
- Service availability

## Performance

### Optimization Strategies
- Redis caching for frequent queries
- Database indexing
- Connection pooling
- Static asset compression

### Typical Performance
- API response time: <100ms
- Database query time: <50ms
- WebSocket update latency: <200ms

## Scaling

### Horizontal Scaling
- Load balancer with multiple API instances
- Database read replicas for queries
- Redis cluster for caching

### Vertical Scaling
- Larger servers with more resources
- Faster storage (SSD)
- Dedicated database server

## Backup & Recovery

### Backup Strategy
- Daily database backups
- Configuration backups
- Retention: 30 days minimum

### Recovery Procedures
- Restore from backup
- Data integrity validation
- Fallback procedures

## Documentation

- [SETUP_GUIDE.md](SETUP_GUIDE.md) - Installation instructions
- [Backend/API_REFERENCE.md](Backend/API_REFERENCE.md) - API endpoints
- [Backend/DATABASE_SCHEMA.md](Backend/DATABASE_SCHEMA.md) - Database structure
- [Backend/AUTHENTICATION.md](Backend/AUTHENTICATION.md) - Auth system
- [Frontend/FEATURES.md](Frontend/FEATURES.md) - Frontend features
- [Deployment/DOCKER_COMPOSE.md](Deployment/DOCKER_COMPOSE.md) - Docker setup
- [Deployment/CLOUD_DEPLOYMENT.md](Deployment/CLOUD_DEPLOYMENT.md) - Production deployment

## Source Code

Cloud application source: `/Product/TrailCurrentCloud/`

---

See also:
- [03_Vehicle_Compute/](../03_Vehicle_Compute/) - Vehicle compute system
- [08_Deployment/](../08_Deployment/) - Deployment procedures
- [09_Troubleshooting/](../09_Troubleshooting/) - Common issues
