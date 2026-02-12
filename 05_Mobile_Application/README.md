# TrailCurrent Android Application

Documentation for the TrailCurrent Android mobile application.

## Overview

The TrailCurrent Android app provides mobile access to the vehicle monitoring and control system:
- Real-time device status and telemetry
- Remote device control
- Notifications and alerts
- Offline support (cached data)
- Vehicle location tracking
- System settings and configuration

## Architecture

```
┌─────────────────────────────┐
│   Android Application       │
├─────────────────────────────┤
│                             │
│ ┌──────────────────────────┐│
│ │  UI Layer                ││
│ │ ├─ Dashboard            ││
│ │ ├─ Device List          ││
│ │ ├─ Control Panel        ││
│ │ └─ Settings             ││
│ └──────────────┬───────────┘│
│                │            │
│ ┌──────────────▼───────────┐│
│ │  Business Logic          ││
│ │ ├─ Device Management    ││
│ │ ├─ Data Caching         ││
│ │ ├─ Offline Support      ││
│ │ └─ Notifications        ││
│ └──────────────┬───────────┘│
│                │            │
│ ┌──────────────▼───────────┐│
│ │  Network Layer           ││
│ │ ├─ REST API Client      ││
│ │ ├─ WebSocket Client     ││
│ │ └─ Authentication       ││
│ └─────────────────────────┘│
│         ↓                  │
│      Network (HTTPS/WSS)   │
│         ↓                  │
└─────────────────────────────┘
```

## Features

### Device Monitoring
- Real-time status display
- Sensor data visualization
- Temperature graphs
- Power consumption tracking
- Location on map (GPS data)

### Device Control
- Power on/off
- Heater control
- Leveler control
- Command queueing
- Favorites for quick access

### Notifications
- Device status alerts
- Command completion notifications
- System warnings
- Customizable notification settings

### Offline Support
- Cached device status
- Offline device list
- Command queuing for when online
- Last known data timestamps

### User Management
- Multi-account support
- Role-based features
- Device sharing (if configured)
- Profile settings

## Setup & Development

### Prerequisites
- Android Studio (latest)
- Android SDK 21+ (but target 33+)
- Gradle
- Java/Kotlin development knowledge
- API endpoint and credentials

### Development Setup

1. Clone the repository from `/Product/TrailCurrentAndroidApp/`
2. Open in Android Studio
3. Configure API endpoints in config files
4. Build and run on emulator or device

See [SETUP_GUIDE.md](SETUP_GUIDE.md) for detailed instructions.

## Technology Stack

| Component | Technology |
|-----------|-----------|
| Language | Kotlin / Java |
| UI Framework | Android Jetpack |
| Navigation | Android Navigation |
| HTTP Client | Retrofit |
| WebSocket | OkHttp/WebSocket |
| Database | SQLite / Room |
| Cache | Shared Preferences |
| Async | Coroutines |

## Project Structure

```
TrailCurrentAndroidApp/
├── app/
│   ├── src/
│   │   ├── main/
│   │   │   ├── java/com/trailcurrent/
│   │   │   │   ├── ui/              # UI components
│   │   │   │   ├── viewmodel/       # Business logic
│   │   │   │   ├── data/            # Data layer
│   │   │   │   ├── network/         # API clients
│   │   │   │   └── utils/           # Utilities
│   │   │   ├── res/
│   │   │   │   ├── layout/          # Activity/Fragment layouts
│   │   │   │   ├── drawable/        # Icons/images
│   │   │   │   ├── values/          # Strings, colors, styles
│   │   │   │   └── menu/            # Menu resources
│   │   │   └── AndroidManifest.xml
│   │   └── test/                    # Unit tests
│   └── build.gradle
└── README.md
```

## API Integration

### REST API Endpoints Used

```
Authentication:
POST /api/auth/login
POST /api/auth/logout
POST /api/auth/refresh

Device Data:
GET /api/devices
GET /api/devices/:id
GET /api/data/:deviceId
GET /api/data/:deviceId/history

Control:
POST /api/commands
GET /api/commands/:id
```

### WebSocket Connection
Real-time updates via WebSocket:
```
wss://[cloud-host]/api/ws
- Device status updates
- New sensor readings
- Command completions
- Notifications
```

## Configuration

### API Configuration
Set in `build.gradle` or local properties:
```gradle
API_BASE_URL=https://api.trailcurrent.cloud
API_WS_URL=wss://api.trailcurrent.cloud
```

### Firebase (Optional)
For push notifications:
- Add Firebase credentials
- Configure notification handling

## Key Activities & Fragments

### Dashboard
Main screen showing:
- Quick status overview
- Favorite devices
- Recent commands
- System alerts

### Device List
Browse all registered devices with:
- Device status
- Last update time
- Quick actions

### Device Detail
View full device information:
- Complete sensor data
- Control commands
- Command history
- Device settings

### Map View
Display vehicle location:
- Real-time position
- Historical track (if available)
- Multiple vehicles
- Route planning

## Data Flow

```
Network Request
  ↓
REST API / WebSocket
  ↓
Response Handler
  ├─ Parse JSON
  ├─ Validate data
  └─ Store in cache
       ↓
ViewModel Update
  └─ Notify UI
       ↓
UI Refresh
  ├─ Update Views
  ├─ Animate changes
  └─ Show notifications
```

## Testing

### Unit Tests
Test business logic:
```bash
./gradlew test
```

### Instrumented Tests
Test on device:
```bash
./gradlew connectedAndroidTest
```

### Manual Testing Checklist
- [ ] Login/logout
- [ ] View device list
- [ ] View device details
- [ ] Send commands
- [ ] Receive WebSocket updates
- [ ] Offline mode functionality
- [ ] Push notifications
- [ ] Settings changes

## Security

### API Security
- HTTPS only
- Certificate pinning recommended
- Token-based authentication
- Encrypted SharedPreferences for credentials

### Data Storage
- Sensitive data encrypted
- Session token expiration
- Clear credentials on logout

### Permissions
- Request required permissions at runtime
- Location (GPS viewing)
- Network (API calls)
- Notifications

## Building & Distribution

### Debug Build
```bash
./gradlew assembleDebug
```

### Release Build
```bash
./gradlew bundleRelease
```

### App Signing
Configure keystore in `gradle.properties`:
```properties
RELEASE_STORE_FILE=path/to/keystore.jks
RELEASE_STORE_PASSWORD=password
RELEASE_KEY_ALIAS=alias_name
RELEASE_KEY_PASSWORD=key_password
```

## Distribution

### Google Play Store
1. Create developer account
2. Prepare app listing
3. Build release APK/AAB
4. Submit for review
5. Publish

### Direct APK Distribution
Distribute APK file directly to users.

## Performance Optimization

### Network
- Request batching
- Connection pooling
- Caching strategies
- Compression

### UI
- Lazy loading
- Image optimization
- View recycling
- Efficient layouts

### Storage
- Database indexing
- Cache cleanup
- Efficient queries

## Debugging

### Logs
View application logs:
```bash
adb logcat | grep trailcurrent
```

### Network Inspection
Use Android Studio Network Profiler to inspect:
- HTTP requests/responses
- WebSocket messages
- Data transmission rates

### Device Inspector
Inspect app data:
```bash
adb shell
cd /data/data/com.trailcurrent
ls -la
```

## Troubleshooting

Common issues:
- Connection timeout → Check API endpoint
- Authentication failure → Verify credentials
- WebSocket disconnect → Check network connectivity
- Crash on startup → Check API response format

## Documentation

- [SETUP_GUIDE.md](SETUP_GUIDE.md) - Development setup
- [FEATURES.md](FEATURES.md) - Complete feature list

## Source Code

Android application source: `/Product/TrailCurrentAndroidApp/`

---

See also:
- [04_Cloud_Application/](../04_Cloud_Application/) - Cloud backend
- [07_Development/](../07_Development/) - Development guidelines
- [09_Troubleshooting/](../09_Troubleshooting/) - Common issues
