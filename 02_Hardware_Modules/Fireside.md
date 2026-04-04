# Fireside - Wireless Touchscreen Display

Central control display for monitoring and controlling devices on the TrailCurrent CAN bus via WiFi and MQTT.

## Hardware

- **Board:** [Waveshare ESP32-P4 WiFi6 Touch LCD 7B](https://www.waveshare.com/esp32-p4-wifi6-touch-lcd-7b.htm?aff_id=Trailcurrent)
- **MCU:** ESP32-P4
- **WiFi:** ESP32-C6 slave via ESP-Hosted (SDIO)
- **Display:** 7" 1024x600 MIPI-DSI LCD with capacitive touch
- **Framework:** ESP-IDF v5.5.2
- **Power:** Battery-powered with wall cradle

## Communication

Fireside does **not** connect directly to the CAN bus. It communicates over WiFi via MQTT (TLS) to the Headwaters vehicle compute, which bridges CAN data to MQTT topics.

### MQTT Subscriptions

| Topic | Data |
|-------|------|
| `local/lights/+/status` | Light on/off state and brightness |
| `local/energy/status` | Battery SOC, voltage, solar watts, consumption, time remaining |
| `local/airquality/temphumid` | Interior temperature and humidity |
| `local/airquality/status` | CO2 (eCO2 ppm) and TVOC (ppb) |
| `local/gps/latlon` | Latitude and longitude |
| `local/gps/alt` | Altitude |
| `local/gps/details` | Satellite count, speed, course, GNSS mode |
| `local/gps/time` | Date and time (year, month, day, hour, minute, second) |

### MQTT Publish

| Topic | Data |
|-------|------|
| `local/lights/{id}/command` | Light toggle and brightness commands |

## SD Card Configuration

WiFi and MQTT credentials are provisioned via an SD card on first boot (or whenever settings need to change).

### Setup

1. Format an SD card as **FAT32**
2. Create a file named **`config.env`** in the root of the SD card
3. Optionally add a **`ca.crt`** file (PEM format) for MQTT TLS certificate verification
4. Insert the SD card and power on (or reboot) the Fireside
5. Once provisioned, the SD card can be removed -- settings are stored in NVS

### config.env Format

Plain text, one `KEY=VALUE` per line. Lines starting with `#` are comments.

```
WIFI_SSID=YourNetworkName
WIFI_PWD=YourWiFiPassword
MQTT_HOST=192.168.1.100
MQTT_PORT=8883
MQTT_USER=mqttuser
MQTT_PASS=mqttpassword
```

| Key | Required | Description |
|-----|----------|-------------|
| `WIFI_SSID` | Yes | WiFi network name |
| `WIFI_PWD` | Yes | WiFi password |
| `MQTT_HOST` | Yes | MQTT broker hostname or IP |
| `MQTT_PORT` | Yes | MQTT broker port (typically 8883 for TLS) |
| `MQTT_USER` | Yes | MQTT username |
| `MQTT_PASS` | Yes | MQTT password |

### ca.crt (Optional)

A PEM-encoded CA certificate file (must be under 8KB). Place it in the root of the SD card alongside `config.env`. Used for verifying the MQTT broker's TLS certificate.

### How It Works

On boot, the firmware:
1. Mounts the SD card (SDMMC Slot 0, 4-bit mode)
2. Reads `config.env` and stores each key/value pair into NVS (namespace `sd_config`)
3. Reads `ca.crt` if present and stores it in NVS
4. Unmounts the SD card and releases power to it

After provisioning, the device reads credentials from NVS on subsequent boots -- the SD card is only needed to update settings.

## Features

- Central dashboard for trailer system monitoring
- Thermostat with temperature control
- Eight device control buttons with on/off and brightness
- GPS, energy, and air quality monitoring
- Color theme switching
- Screen brightness and timeout controls
- Timezone selection
- All user settings persisted in NVS

## Source Code

Repository: `Product/TrailCurrentFireside/`
