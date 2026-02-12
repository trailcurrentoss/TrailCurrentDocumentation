# In-Vehicle Compute System Setup Guide

Step-by-step guide to set up the edge computing system for TrailCurrent.

This guide uses Raspberry Pi 4/5 as the reference implementation, but the same principles apply to other Linux-based compute devices (Orange Pi, Jetson Nano, etc.).

## Prerequisites

- Edge compute device (Raspberry Pi 4 or 5 recommended - 4GB RAM minimum)
  - Alternative devices: Orange Pi, Jetson Nano, or similar Linux-based SBC
- Storage media (MicroSD card, eMMC, SSD - 32GB+ recommended)
- Power supply (appropriate for your device)
- Ethernet cable or WiFi connectivity
- CAN transceiver board (SPI or GPIO-based)
- USB UART adapter (optional, for debugging)

## Step 1: Install Operating System

### 1.1 Prepare Storage Media

**For Raspberry Pi:**
1. Download [Raspberry Pi Imager](https://www.raspberrypi.com/software/)
2. Insert storage media into computer
3. Launch Raspberry Pi Imager
4. Choose OS: **Raspberry Pi OS Lite** (64-bit)
5. Choose storage: Your media

**For Other Devices (Orange Pi, Jetson Nano, etc.):**
- Use appropriate OS image and flashing tool for your device
- Recommended: Ubuntu Server 20.04 LTS or 22.04 LTS (64-bit)

### 1.2 Configure System

1. Set hostname: `trailcurrent-vehicle` (or desired name)
2. Enable SSH (password or key auth)
3. Create user account
4. Configure networking (Ethernet or WiFi)
5. Set timezone
6. Write image to storage media

### 1.3 First Boot

1. Insert storage media into device
2. Connect ethernet cable (or WiFi)
3. Power on device
4. Wait 2-3 minutes for boot

### 1.4 SSH Access

```bash
# Find device IP address
ping trailcurrent-vehicle.local

# SSH into device
ssh [username]@trailcurrent-vehicle.local
# Or: ssh [username]@[device-ip-address]
```

## Step 2: Configure CAN Interface

### 2.1 Enable SPI (if using SPI CAN transceiver)

```bash
sudo raspi-config
# Navigate to: Interfacing Options > SPI > Yes
# Reboot
```

### 2.2 Install CAN Tools

```bash
sudo apt-get update
sudo apt-get install -y can-utils
```

### 2.3 Configure CAN Interface

Create file `/etc/network/interfaces.d/can0`:

```bash
sudo nano /etc/network/interfaces.d/can0
```

Add content:
```
auto can0
iface can0 can static
    bitrate 500000
    restart-ms 100
```

Bring up CAN interface:
```bash
sudo ip link set can0 type can bitrate 500000
sudo ip link set can0 up

# Verify
ip link show can0
candump can0  # Should show CAN traffic
```

## Step 3: Install Docker

### 3.1 Install Docker Engine

```bash
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh

# Add user to docker group
sudo usermod -aG docker pi
# (Log out and back in for group to take effect)
```

### 3.2 Install Docker Compose

```bash
sudo apt-get install -y docker-compose
# Or if using Compose v2:
sudo apt-get install -y docker-compose-plugin
```

### 3.3 Verify Installation

```bash
docker --version
docker-compose --version
docker ps
```

## Step 4: Deploy Services

### 4.1 Clone Repository

```bash
cd /home/pi
git clone https://[gitea-server]/TrailCurrent/TrailCurrentPiCanToMqttAndDocker.git
cd TrailCurrentPiCanToMqttAndDocker
```

### 4.2 Configure Environment

Create `.env` file:

```bash
cp .env.example .env
nano .env
```

Edit variables:
- `CAN_INTERFACE=can0`
- `MQTT_BROKER=localhost`
- `CLOUD_API_URL=https://[cloud-server]`
- `VEHICLE_ID=[your-vehicle-id]`
- `API_KEY=[your-api-key]`

### 4.3 Start Docker Services

```bash
docker-compose up -d

# Verify services are running
docker-compose ps

# View logs
docker-compose logs -f can-gateway
```

## Step 5: Test & Validate

### 5.1 Test CAN Bus Connection

```bash
# Monitor CAN traffic
candump can0

# Send test message (from another device)
cansend can0 123#1234567890
```

### 5.2 Test MQTT

```bash
# Subscribe to topic
docker exec mosquitto mosquitto_sub -h localhost -t tc/gps/position

# From another container/device, publish:
docker exec mosquitto mosquitto_pub -h localhost -t tc/gps/position -m '{"lat": 47.25, "lon": -122.44}'
```

### 5.3 Test API Server

```bash
# Check if API is responding
curl http://localhost:3000/api/health

# View logs
docker-compose logs api
```

### 5.4 Test Cloud Connectivity

```bash
# Check if cloud MQTT is connected
docker-compose logs can-gateway | grep "cloud\|connected"

# Verify data flowing to cloud
curl https://[cloud-server]/api/devices
```

## Step 6: System Configuration

### 6.1 Set Up Persistent Storage

```bash
# Create data directory
mkdir -p /opt/trailcurrent/data
sudo chown pi:pi /opt/trailcurrent/data

# Configure docker-compose volumes
# (Already in docker-compose.yml)
```

### 6.2 Enable Auto-Start on Boot

Create systemd service `/etc/systemd/system/docker-compose-app.service`:

```bash
sudo nano /etc/systemd/system/docker-compose-app.service
```

Add content:
```ini
[Unit]
Description=Docker Compose Application
After=docker.service
Requires=docker.service

[Service]
Type=oneshot
WorkingDirectory=/home/pi/TrailCurrentPiCanToMqttAndDocker
ExecStart=/usr/bin/docker-compose up -d
ExecStop=/usr/bin/docker-compose down
RemainAfterExit=yes
User=pi

[Install]
WantedBy=multi-user.target
```

Enable service:
```bash
sudo systemctl daemon-reload
sudo systemctl enable docker-compose-app
sudo systemctl start docker-compose-app
```

### 6.3 Configure System Resources

Increase open files limit:

```bash
sudo nano /etc/security/limits.conf
```

Add:
```
pi soft nofile 65536
pi hard nofile 65536
```

## Step 7: Monitoring & Maintenance

### 7.1 Check System Health

```bash
# Check disk usage
df -h

# Check memory usage
free -h

# Check container status
docker ps

# Check system temperature
vcgencmd measure_temp
```

### 7.2 Backup Configuration

```bash
# Backup docker-compose config
cp docker-compose.yml docker-compose.yml.backup

# Backup environment
cp .env .env.backup

# Backup data
tar -czf data-backup.tar.gz /opt/trailcurrent/data/
```

### 7.3 Update System

```bash
sudo apt-get update
sudo apt-get upgrade -y

# Update Docker images
docker-compose pull
docker-compose up -d
```

## Troubleshooting

### CAN Interface Not Found

```bash
# Check if interface exists
ip link show can0

# If not, reload kernel module
sudo modprobe mcp251x

# If still not working, check pinout and wiring
```

### Docker Service Won't Start

```bash
# Check Docker status
sudo systemctl status docker

# View Docker logs
sudo journalctl -u docker -f

# Restart Docker
sudo systemctl restart docker
```

### Out of Memory

```bash
# Check available memory
free -h

# Stop unnecessary containers
docker stop [container-name]

# Increase swap (if needed)
```

### No Internet Connection

```bash
# Check network status
ip addr show

# Check routing
route -n

# Restart network services
sudo systemctl restart networking
```

## Advanced Configuration

### Custom CAN Settings

Edit `/etc/network/interfaces.d/can0`:
```
iface can0 can static
    bitrate 1000000  # 1 Mbps instead of 500 kbps
    sample-point 75
```

### Enable Hardware Watchdog

Useful for production deployments to auto-reboot on hang:

```bash
sudo nano /etc/modprobe.d/bcm2835_wdt.conf
# Add: options bcm2835_wdt heartbeat=10

sudo systemctl enable watchdog
```

### Configure Firewall

```bash
# Install UFW
sudo apt-get install -y ufw

# Allow SSH
sudo ufw allow 22

# Allow internal services only
sudo ufw allow from 192.168.1.0/24 to any port 3000

# Enable firewall
sudo ufw enable
```

## Next Steps

1. [Configure containers](Configuration/CONFIG_FILES.md)
2. [Add hardware modules](../02_Hardware_Modules/README.md)
3. [Connect to cloud](../04_Cloud_Application/README.md)
4. [Monitor system](Deployment/PI_DEPLOYMENT.md)

---

See also:
- [README.md](README.md) - Vehicle Compute overview
- [DOCKER_CONTAINERS.md](DOCKER_CONTAINERS.md) - Container details
- [Configuration/CONFIG_FILES.md](Configuration/CONFIG_FILES.md) - App configuration
