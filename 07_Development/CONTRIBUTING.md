# Contributing to TrailCurrent

Thank you for wanting to contribute to TrailCurrent! This document outlines how to contribute while preserving the project's core mission and principles.

## ⚠️ Legal Acknowledgment for Contributors

**By contributing to TrailCurrent, you acknowledge that:**

1. TrailCurrent is a reference implementation provided AS-IS without warranties
2. You have read and understand [LIABILITY_AND_DISCLAIMER.md](../../LIABILITY_AND_DISCLAIMER.md)
3. Your contributions do not create liability for you or original creators for user outcomes
4. Users accept all responsibility for their use of your contributions
5. All contributions must maintain safety-first principles (physical wired controls required for critical functions)

**Contributors cannot be held liable** for how others use their code. By contributing, you ensure your code follows project principles and maintains safety requirements, but accept no liability for user implementation or outcomes.

---

## Core Mission

TrailCurrent is built on **three fundamental principles** that **must be preserved in every contribution**. If you're new to the project, visit [trailcurrent.com](https://trailcurrent.com) to understand the platform's goals and [architecture](https://trailcurrent.com/architecture.html).

### 1. ⚪ Cloud is Optional
- The system must work **completely independently** without any cloud connection
- Users should never be forced or pressured to use cloud services
- Cloud features are purely **opt-in enhancements**
- Test all features in **offline mode** first

### 2. 🔐 Data Privacy First
- Vehicle data must **never be sent to third-party services** without explicit user consent
- **NO Big Tech APIs** allowed (AWS, Google Cloud, Azure, etc.) without user awareness
- **NO tracking, telemetry, or analytics** that phone home
- Users must have **complete control** over where their data goes
- If you propose a cloud feature, it must be **self-hostable and private**

### 3. 📖 100% Open Source
- All code, firmware, designs, and tools must be **openly licensed**
- No proprietary components or closed-source dependencies
- No license lock-in or vendor restrictions
- Include source code for all CAD files, EDA schematics, and designs
- Use only open-source tools and standards

## Before You Code

**Ask yourself:**
- Does this feature require the cloud? If yes, is it truly optional?
- Does this feature send data anywhere? If yes, is the user in control?
- Does this add a proprietary dependency? If yes, is there an open-source alternative?
- Would this break offline functionality? If yes, reconsider the design.

**If the answer to any concern is unclear, discuss it in an issue first.**

## Code Guidelines

### For All Code

1. **Maintain Offline-First Design**
   - All features must work locally first
   - Cloud sync is secondary
   - No internet required for core functionality

2. **No External Dependencies**
   - Avoid proprietary APIs and cloud services
   - If a service is needed, it must be self-hostable
   - Document why the dependency is necessary
   - Provide open-source alternatives when possible

3. **Privacy by Default**
   - No data collection without explicit user consent
   - No telemetry or diagnostics that leave the vehicle
   - No analytics or crash reporting to external services
   - Users control their data completely

4. **Open Source Everything**
   - All dependencies must have compatible open-source licenses
   - No trial/freemium services with paywalls
   - No SaaS-only tools without offline alternatives

### Hardware/CAD Files

- Include **editable source files** (not just rendered models)
- Use open-source tools (KiCad for EDA, FreeCAD for mechanical)
- Document all part numbers and sources
- No proprietary hardware requirements

### Firmware (C/C++)

- Use **ESP-IDF** (open-source framework)
- No proprietary libraries or SDKs
- Document all CAN messages and MQTT topics
- Ensure OTA updates work independently

### Control Modules (Critical Safety Requirement!)

⚠️ **ANY module that controls vehicle critical functions (motors, fans, lights, heaters, levelers, etc.) MUST have:**

1. **Primary: Wired CAN Bus Control**
   - CAN interface for reliable hardwired control
   - No wireless dependency for critical operations
   - Works even if all other systems fail

2. **Secondary: Physical Controls**
   - Hardwired switches or buttons
   - Manual override capability
   - No network required for basic operation

3. **Optional: Cloud/WiFi Enhancement**
   - ONLY after wired control is implemented
   - NEVER the only control method
   - ALWAYS has fallback to physical control

**Examples:**
- ✅ Heater: Physical switch + CAN control + optional WiFi
- ✅ Leveler: Physical panel buttons + CAN control + optional remote
- ✅ Fan: Hardwired relay + CAN control + optional automation
- ❌ Any device with ONLY WiFi/Cloud control

### Backend (Node.js)

- Use **Express.js** with open-source packages
- No cloud vendor lock-in (no Firebase, DynamoDB, etc.)
- All data storage must be user-controllable (PostgreSQL, SQLite, etc.)
- No external authentication services required (optional: OAuth, but with self-hosted option)
- **For control features**: ensure local CAN/MQTT control works even if cloud is down

### Frontend

- Pure **HTML/CSS/JavaScript** (no heavy framework lock-in)
- Works **offline with local MQTT**
- No external CDNs required for core functionality
- No telemetry or analytics

## Submitting Changes

### Pull Request Process

1. **Fork and branch** - Create a feature branch from `develop`
2. **Code following guidelines** - Ensure all principles are maintained
3. **Test offline** - Verify features work without internet
4. **Document changes** - Update relevant documentation
5. **Commit with good messages** - Explain the why, not just the what
6. **Create PR** - Link related issues, explain your changes

### PR Description Template

```markdown
## Summary
[1-2 sentence description of changes]

## Why This Change?
[Explain the problem being solved]

## Design Considerations
- [ ] Offline functionality maintained
- [ ] No external API dependencies
- [ ] No data sent to third parties
- [ ] 100% open source

## Testing
[Describe how you tested this]

## Offline Testing
[Verify this works without internet]

## Documentation
[Any docs that need updating]
```

## Code Review Checklist

Reviewers will check:

- ✅ Offline functionality - Does it work without cloud?
- ✅ Data privacy - Is any data sent externally? If so, is user in control?
- ✅ Open source - Are all dependencies open source?
- ✅ No vendor lock-in - Is this tied to a specific provider?
- ✅ Documentation - Are changes documented?
- ✅ Architecture - Does this align with platform design?

## Issues You Can Help With

Look for issues tagged with:
- `good-first-issue` - Great for new contributors
- `help-wanted` - Community input needed
- `documentation` - Improve docs and guides
- `offline-mode` - Enhance offline capabilities
- `open-source` - Add open-source alternatives

## Reporting Bugs

When reporting bugs, include:

```
**Offline/Online?** (Where did this happen?)
**Steps to reproduce:**
1. ...

**Expected behavior:**
[What should happen]

**Actual behavior:**
[What happened]

**Environment:**
- Hardware: [Raspberry Pi 4, etc.]
- OS: [Raspberry Pi OS, Ubuntu, etc.]
- Internet: [Connected/Disconnected]
```

## Questions?

- **Architecture questions** - Open an issue to discuss design
- **Privacy concerns** - Always flag potential data leaks
- **Open source issues** - Report proprietary dependencies
- **General questions** - Join our community forums

## Code of Conduct

Be respectful, inclusive, and thoughtful. We welcome contributions from all backgrounds and experience levels.

## License

All contributions must be licensed under the MIT License. By submitting code, you agree to this licensing.

## Special Notes for New Developers

⚠️ **IMPORTANT** - Please read and understand these three principles:

1. **Cloud is optional** - test offline first
2. **Data is private** - never send to third parties
3. **Everything is open source** - no proprietary code or tools

If you're unsure whether a feature aligns with these principles, **ask in an issue first**. It's better to discuss before coding than to have a PR rejected.

## Thank You!

Your contributions help make TrailCurrent better while preserving the values that make it special - privacy, independence, and open source.
