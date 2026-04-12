# TrailCurrent Mobile Applications

TrailCurrent has two first-party mobile clients. Both talk to the same Farwatch
REST + WebSocket API — pick whichever you prefer or whichever you want to
contribute to.

| App | Source | Platform | Framework | Language |
|-----|--------|----------|-----------|----------|
| **TrailCurrent Outbound** | `/Product/TrailCurrentOutbound/` | Android native | Jetpack Compose + Material 3 | Kotlin |
| **TrailCurrent App** (React Native) | `/Product/TrailCurrentReactNativeApp/` | iOS + Android | Expo SDK 54 + React Native 0.81 | TypeScript |

Both apps authenticate against Farwatch using an **API key** carried in the
`Authorization: Bearer` header, and subscribe to live state over WebSocket.

---

## TrailCurrent Outbound (Android Native)

Native Android app for the TrailCurrent monitoring and control system.

### Features
- **Home Dashboard** — Nest-style thermostat dial + light grid with brightness adjust
- **Trailer Monitoring** — bubble level indicators, GNSS details (satellites, heading)
- **Energy Monitoring** — Solar input, battery SoC, charge status
- **Water Tanks** — fresh / grey / black tank levels (from Reservoir)
- **Air Quality** — temperature, humidity, IAQ, CO2 (from Borealis)
- **Map View** — real-time vehicle location on MapLibre GL vector tiles with 2D/3D modes and compass
- **Settings** — server config, dark/light theme, timezone, clock format

### Tech Stack
- **Language:** Kotlin
- **UI:** Jetpack Compose + Material 3
- **Architecture:** MVVM + Repository pattern
- **DI:** Hilt
- **Networking:** Retrofit + OkHttp (REST), OkHttp WebSocket (real-time)
- **Local Storage:** DataStore Preferences
- **Maps:** MapLibre GL (vector tiles from Farwatch's tileserver-gl)
- **Auth:** API key via `Authorization` header
- **Minimum SDK:** 26 (Android 8.0)
- **Target SDK:** 34 (Android 14)

### Build

```bash
cd /Product/TrailCurrentOutbound
./gradlew assembleDebug        # debug APK
./gradlew bundleRelease        # release AAB for Play Store
```

Configure the Farwatch host and API key in the in-app Settings screen.

---

## TrailCurrent App (React Native / Expo)

Cross-platform mobile app for monitoring and controlling TrailCurrent vehicle
systems. Builds for iOS and Android from a single codebase.

### Features
- **Home** — thermostat dial (drag to adjust) + light grid (tap toggle, long-press brightness)
- **Trailer** — bubble level indicators + GNSS details (satellites, speed, course)
- **Energy** — solar input, battery level/voltage, charge status, time remaining
- **Water** — fresh / grey / black tank levels with color-coded thresholds
- **Map** — real-time vehicle position

### Tech Stack
| Layer | Technology |
|-------|------------|
| Framework | Expo SDK 54, React Native 0.81, TypeScript |
| Routing | Expo Router (file-based) |
| State | React Context + `useReducer` |
| HTTP | built-in `fetch` |
| WebSocket | built-in WebSocket with auto-reconnect |
| Storage | AsyncStorage (prefs), SecureStore (API key) |
| Maps | `react-native-maps` |
| Icons | `@expo/vector-icons` (Ionicons) |

### Build

```bash
cd /Product/TrailCurrentReactNativeApp
npm install
npx expo start                   # development server
eas build --platform ios         # Expo Application Services cloud build
eas build --platform android
```

Configure the Farwatch host and API key through the in-app Settings tab. The
API key is persisted to SecureStore (Keychain on iOS, EncryptedSharedPreferences
on Android).

---

## Security Notes (both apps)

- **HTTPS only** — certificate pinning recommended for production builds
- **API key storage** — Android DataStore + SecureStore / Keychain, never plaintext
- **Token revocation** — Farwatch API keys can be revoked at any time without
  affecting the vehicle itself (the vehicle continues to run locally)

---

See also:
- [04_Cloud_Application/](../04_Cloud_Application/) — Farwatch cloud backend
- [03_Vehicle_Compute/](../03_Vehicle_Compute/) — Headwaters (vehicle compute)
- [07_Development/](../07_Development/) — Development guidelines
