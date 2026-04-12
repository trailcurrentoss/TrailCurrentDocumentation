# TrailCurrent Farwatch (Cloud Platform)

Complete guide to **TrailCurrent Farwatch**, the optional cloud platform for remote monitoring and control of a vehicle running TrailCurrent Headwaters.

> **Reminder — [Cloud Optional Core Principle](../CORE_PRINCIPLES.md):** Farwatch is
> always optional. A TrailCurrent vehicle is fully functional without it. Farwatch
> never becomes a single point of failure, never holds a user's vehicle hostage,
> and stores as little data as possible.

## Overview

Farwatch is a cloud-hosted **Progressive Web App (PWA)** that mirrors the local
Headwaters dashboard over the public internet. It provides:

- A responsive web UI accessible from any modern browser
- Remote thermostat, lighting, relay, leveler, and sensor monitoring
- GPS / map view with offline-capable vector tiles (MapLibre GL)
- Deployment package hosting for module firmware (resumable downloads)
- Proximity-based automation (phone-to-vehicle arrival / departure triggers)
- API key management for programmatic access
- Real-time updates over WebSocket bridged from the vehicle's MQTT broker

Farwatch is the cloud endpoint that the [mobile applications](../05_Mobile_Application/README.md)
and the browser-based PWA talk to. The vehicle connects outward over MQTTS — the
cloud never initiates a connection inward.

## Architecture

Dockerized microservices stack — the same topology as Headwaters, extended with
public HTTPS and cross-vehicle routing:

```
┌──────────────────────────────────────────────────────────────┐
│                    Cloud Host (any VPS)                      │
├──────────────────────────────────────────────────────────────┤
│                                                              │
│  Public HTTPS (Let's Encrypt)                                │
│         ↓                                                    │
│  ┌────────────────────────────────────────────────────────┐  │
│  │  Frontend (nginx)                                      │  │
│  │    • Vanilla JS PWA                                    │  │
│  │    • WebSocket bridge for live updates                 │  │
│  └────────┬───────────────────────────────────────────────┘  │
│           │                                                  │
│  ┌────────┴───────────────────────────────────────────────┐  │
│  │  Backend (Node.js / Express)                           │  │
│  │    • REST API                                          │  │
│  │    • WebSocket fan-out from MQTT                       │  │
│  │    • API key authentication                            │  │
│  │    • Deployment package storage                        │  │
│  │    • Proximity automation engine                       │  │
│  └────┬───────────────────────┬───────────────────────────┘  │
│       │                       │                              │
│  ┌────┴────┐           ┌──────┴─────┐                        │
│  │ MongoDB │           │  Mosquitto │ ← MQTTS 8883           │
│  │ (state) │           │   broker   │                        │
│  └─────────┘           └──────┬─────┘                        │
│                               │                              │
│  ┌─────────────────────────────┴─────┐                       │
│  │  Tileserver (vector tiles)        │                       │
│  └───────────────────────────────────┘                       │
└──────────────────────────────────────┬───────────────────────┘
                                       │ MQTTS over cellular
                                       ▼
                           ┌────────────────────────┐
                           │ Headwaters (in vehicle)│
                           │   bridged topics       │
                           └────────────────────────┘
```

## Technology Stack

| Layer     | Technology                                 | Purpose                                              |
|-----------|--------------------------------------------|------------------------------------------------------|
| Frontend  | nginx + vanilla JS PWA                     | Responsive dashboard, offline support                |
| Backend   | Node.js + Express                          | REST API + WebSocket bridge + automation engine      |
| Database  | MongoDB                                    | Settings, state snapshots, deployment metadata       |
| Messaging | Mosquitto MQTT (TLS)                       | Bridge between cloud and vehicle Headwaters          |
| Tiles     | tileserver-gl                              | Offline-capable MapLibre vector tiles                |
| Hosting   | Docker Compose                             | Containerized deployment on any VPS                  |
| TLS       | Let's Encrypt (via certbot container)      | Public HTTPS                                         |

## Key Features

### Dashboard

- **Thermostat Control** — read live temperature / humidity from Borealis, adjust
  Therma setpoint and thresholds
- **Lighting** — toggle and dim the 8 Torrent PDM channels; drive light sequences
- **Energy Dashboard** — battery voltage, SoC %, solar harvest, charge status,
  time-remaining from Ampline + Solstice
- **Water Tanks** — fresh / grey / black levels from Reservoir
- **Air Quality** — temperature, humidity, IAQ index, CO2 from Borealis
- **Trailer Level** — pitch / roll from Plateau
- **GPS / Map** — real-time location on MapLibre vector map tiles
- **Trailer Monitor** — Aftline 7-pin connector status

### Proximity Automation

Phone-to-vehicle proximity detection with configurable zone radii, device
registration, and automation rules that can trigger Torrent or Switchback actions
on arrival or departure. Useful for "turn on the awning lights when I get within
50 m of the trailer" style workflows.

### Deployment Packages

Farwatch acts as the OTA distribution origin for module firmware. Operators
upload firmware packages, track versions per module, and deliver them to
Headwaters devices over resumable downloads. Headwaters then distributes the
new binaries to the appropriate modules over CAN (OTA trigger) and WiFi
(HTTP binary upload) — see [08_Deployment/FIRMWARE_UPDATES.md](../08_Deployment/FIRMWARE_UPDATES.md).

### API Keys

Instead of user accounts, Farwatch uses API keys for authentication. Each phone,
browser, or integration carries its own key in an `Authorization` header. Keys
are created, rotated, and revoked through the dashboard.

## Setup & Installation

### Prerequisites

- A VPS with Docker and Docker Compose
- A public DNS name pointed at the host (required for Let's Encrypt)
- Outbound MQTTS access from the in-vehicle Headwaters

### Quick Start

```bash
git clone https://github.com/trailcurrent/TrailCurrentFarwatch.git
cd TrailCurrentFarwatch
cp .env.example .env       # set TC_DOMAIN, MQTT credentials, etc.
docker compose up -d
```

See [Deployment/CLOUD_DEPLOYMENT.md](Deployment/CLOUD_DEPLOYMENT.md) for the
production deployment checklist and TLS certificate setup.

## REST API (Selected Endpoints)

All endpoints require an `Authorization: Bearer <api_key>` header.

**Lights / Relays**
- `PUT /api/lights/:id` — toggle / set brightness on a Torrent channel
- `PUT /api/relays/:id` — toggle a Switchback channel
- `GET /api/lights` — current state snapshot

**Thermostat**
- `GET /api/thermostat` — current Therma status (from `0x40 ThermaStatus`)
- `PUT /api/thermostat/setpoint` — sends `0x41 ThermaSetDesiredRequest`
- `PUT /api/thermostat/threshold` — sends `0x42 ThermaSetThresholdRequest`

**Energy / Water / Sensors**
- `GET /api/energy` — latest Ampline + Solstice snapshot
- `GET /api/water` — latest Reservoir tank levels
- `GET /api/airquality` — latest Borealis reading
- `GET /api/level` — latest Plateau tilt and corner data
- `GET /api/gps` — latest Bearing position

**Automation**
- `GET/POST/DELETE /api/proximity/devices` — manage registered phones
- `GET/POST/DELETE /api/automations` — manage automation rules

**Deployment**
- `GET /api/deployment/packages` — list firmware packages
- `POST /api/deployment/packages` — upload a firmware package
- `GET /api/deployment/download/:id` — resumable download (Range header supported)

## WebSocket

`wss://<host>/api/ws` streams change-only updates that originated on the vehicle
MQTT broker. Each message carries the MQTT topic and payload that triggered it.

## Security

- **HTTPS only**, Let's Encrypt via a certbot container
- **API key** auth with rotation and revocation (no passwords / JWT)
- **MQTT over TLS** (port 8883) between cloud and vehicle
- **No user PII** stored by default — Farwatch only holds settings, deployment
  metadata, and transient state required for the dashboard
- **No long-term telemetry retention** — Farwatch forwards WebSocket updates
  without persisting sensor history (see `DATA_FLOW.md` scenario 1)

## Data Residency

Farwatch intentionally stores as little vehicle data as possible:
- **Stored:** user settings, automation rules, API keys, firmware deployment metadata
- **Not stored (by default):** sensor history, GPS tracks, camera streams

See [CORE_PRINCIPLES.md](../CORE_PRINCIPLES.md) for the Data Privacy First
rationale.

## Source Code

Farwatch source: `/Product/TrailCurrentFarwatch/`

---

See also:
- [03_Vehicle_Compute/](../03_Vehicle_Compute/) — Headwaters (in-vehicle compute)
- [05_Mobile_Application/](../05_Mobile_Application/) — Outbound (Android) and the React Native app
- [08_Deployment/CLOUD_DEPLOYMENT.md](../08_Deployment/) — Deployment procedures
- [09_Troubleshooting/](../09_Troubleshooting/) — Common issues
