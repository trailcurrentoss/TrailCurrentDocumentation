# TrailCurrent Development Guide

Complete guide for developing and contributing to the TrailCurrent platform.

## Overview

This section covers:
- Setting up the development environment
- Building and testing components
- Development workflows and best practices
- Contributing guidelines
- Debugging techniques

## Development Paths

### Hardware Module Development
For ESP32 firmware:
1. [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md) - Set up build environment
2. [BUILD_SYSTEM.md](BUILD_SYSTEM.md) - Understand CMake and build
3. Choose module to develop
4. Follow module documentation
5. [TESTING_GUIDE.md](TESTING_GUIDE.md) - Write and run tests
6. [DEBUGGING.md](DEBUGGING.md) - Debug issues

### Cloud Application Development
For backend/frontend:
1. [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md) - Set up Node.js environment
2. Read cloud app documentation
3. Set up database and services
4. Develop features
5. [TESTING_GUIDE.md](TESTING_GUIDE.md) - Run test suite
6. Submit pull request

### In-Vehicle Compute Development
For edge computing system:
1. [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md) - Set up Docker environment
2. Review architecture
3. Modify Docker services
4. Test locally on Pi
5. Validate with cloud

### Mobile App Development
For Android application:
1. [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md) - Set up Android Studio
2. Clone and open project
3. Configure API endpoints
4. Develop features
5. [TESTING_GUIDE.md](TESTING_GUIDE.md) - Test on device/emulator

## Quick Start

### 1. Clone Repository
```bash
git clone https://[gitea-server]/TrailCurrent/[component]
cd [component]
```

### 2. Choose Your Path
- **Hardware**: See [BUILD_SYSTEM.md](BUILD_SYSTEM.md)
- **Cloud**: Install Node.js 24+, run `npm install`
- **Pi**: Install Docker, run `docker-compose up`
- **Mobile**: Open Android Studio

### 3. Read Component Documentation
See `/04_Cloud_Application/`, `/03_Vehicle_Compute/`, etc.

### 4. Make Changes
Follow coding standards and best practices

### 5. Test Your Work
[TESTING_GUIDE.md](TESTING_GUIDE.md) - Run appropriate tests

### 6. Submit for Review
[CONTRIBUTING.md](CONTRIBUTING.md) - Follow contribution process

## Development Tools

### Required

| Component | Tool | Version |
|-----------|------|---------|
| Hardware | ESP-IDF | 5.0+ |
| Cloud | Node.js | 24+ |
| Cloud | PostgreSQL | 12+ |
| Cloud | Redis | 6+ |
| Mobile | Android Studio | Latest |
| Pi | Docker | 20.10+ |

### Recommended

- **Version Control**: Git + Gitea
- **IDE**: VS Code (hardware/cloud), Android Studio (mobile)
- **Database**: DBeaver or pgAdmin (PostgreSQL)
- **API Testing**: Postman or Insomnia
- **Monitoring**: Docker Desktop

## Documentation Structure

- [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md) - Environment setup for all components
- [BUILD_SYSTEM.md](BUILD_SYSTEM.md) - How to build everything
- [TESTING_GUIDE.md](TESTING_GUIDE.md) - Testing procedures
- [DEBUGGING.md](DEBUGGING.md) - Debugging tips and techniques
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines

## Common Tasks

### Building a Hardware Module
```bash
cd /Product/TrailCurrentGpsModule
idf.py set-target esp32
idf.py build
idf.py flash
idf.py monitor
```

### Running Cloud Locally
```bash
cd /Product/TrailCurrentCloud
docker-compose -f docker-compose.dev.yml up
# API: http://localhost:3000
# Frontend: http://localhost:8080
```

### Deploying to Pi
```bash
cd /Product/TrailCurrentPiCanToMqttAndDocker
./deploy.sh [pi-ip-address]
```

### Building Android App
```bash
cd /Product/TrailCurrentAndroidApp
./gradlew assembleDebug
# APK: app/build/outputs/apk/debug/
```

## Development Environment

### Directory Structure
```
/Product/
├── Libraries/              # Shared libraries
├── TrailCurrent*/          # Hardware modules
├── TrailCurrentCloud/      # Cloud application
├── TrailCurrentPiCanToMqttAndDocker/  # Vehicle Pi
├── TrailCurrentAndroidApp/ # Mobile app
└── TrailCurrentDocumentation/  # This documentation
```

### Development Branches

Follow git branching strategy:
- `main` - Production releases
- `develop` - Development integration
- `feature/[name]` - New features
- `fix/[name]` - Bug fixes
- `docs/[name]` - Documentation updates

### Commit Messages

Write clear commit messages:
```
[TYPE] Short description (max 70 chars)

Longer explanation if needed (72 char line wrap)

- Bullet points for changes
- Another change

Closes #123
```

Types: feat, fix, docs, style, refactor, test, chore

## Code Quality

### Standards
- Follow project coding style
- Write readable, self-documenting code
- Add comments for complex logic
- Keep functions small and focused
- Use meaningful variable names

### Pre-commit Checks
- Code formatting
- Linting
- Unit tests pass
- No console errors

### Review Process
All changes require review before merging:
1. Create feature branch
2. Make changes
3. Write tests
4. Submit pull request
5. Respond to review feedback
6. Squash and merge

## Debugging

### Hardware Modules
- Monitor serial output: `idf.py monitor`
- Enable debug logging
- Use GDB debugger (if supported)

### Cloud Application
- Check logs: `npm run logs` or `docker logs`
- Use breakpoints in VS Code
- Check database queries

### Vehicle Pi
- SSH into Pi: `ssh pi@[address]`
- View container logs: `docker logs [container]`
- Check system logs: `journalctl -u docker -f`

### Mobile App
- Android Studio debugger
- Logcat output
- Network inspector

See [DEBUGGING.md](DEBUGGING.md) for detailed techniques.

## Testing

### Unit Tests
```bash
npm test              # Cloud
./gradlew test        # Mobile
idf.py pytest         # Hardware (if configured)
```

### Integration Tests
```bash
npm run test:e2e      # Cloud
./gradlew connectedAndroidTest  # Mobile
```

### Manual Testing
- Hardware: Test on actual board
- Cloud: Test with vehicle simulator
- Mobile: Test on device and emulator
- Pi: Test with real hardware modules

See [TESTING_GUIDE.md](TESTING_GUIDE.md) for comprehensive testing guide.

## Performance Profiling

### Hardware
- Monitor RAM/Flash usage: `idf.py size`
- Profile execution time
- Measure power consumption

### Cloud
- Node.js profiling tools
- Database query performance
- Memory leak detection

### Mobile
- Android Profiler
- Memory usage
- Network bandwidth

## Documentation

When developing:
1. Update relevant docs
2. Add code comments for complex logic
3. Update API documentation
4. Add configuration examples
5. Include troubleshooting tips

## Getting Help

### Resources
- Component-specific READMEs
- Architecture documentation
- Existing code examples
- Team communication channels

### Common Issues
See [09_Troubleshooting/COMMON_ISSUES.md](../09_Troubleshooting/COMMON_ISSUES.md)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for:
- How to submit changes
- Code review process
- Commit conventions
- Pull request format

## Next Steps

1. [Set up development environment](DEVELOPMENT_SETUP.md)
2. [Read build system guide](BUILD_SYSTEM.md)
3. [Choose component to work on](../README.md)
4. [Follow contribution guidelines](CONTRIBUTING.md)

---

See also:
- [DEVELOPMENT_SETUP.md](DEVELOPMENT_SETUP.md) - Environment setup
- [BUILD_SYSTEM.md](BUILD_SYSTEM.md) - Build procedures
- [TESTING_GUIDE.md](TESTING_GUIDE.md) - Testing strategies
- [DEBUGGING.md](DEBUGGING.md) - Debugging techniques
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution process
