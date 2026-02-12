# TrailCurrent Core Principles

## Software Defined Vehicle Platform with Autonomous Intelligence

TrailCurrent is an **Open Source Software Defined Vehicle (SDV) Platform with Autonomous Intelligence** - these core principles define what makes it fundamentally different from generic IoT platforms. They ensure TrailCurrent remains a **vehicle-focused, safety-first, autonomously-intelligent, and user-controlled system**.

**Every architectural decision, feature, and contribution must respect these principles.**

**Key Distinction**: Unlike cloud-dependent AI or manual control systems, TrailCurrent features intelligent local decision-making and autonomous actions that run completely on-vehicle, offline-capable, and under user control.

⚖️ **IMPORTANT**: These are design principles for the project. For legal information about liability and disclaimers, see [LIABILITY_AND_DISCLAIMER.md](LIABILITY_AND_DISCLAIMER.md). Users accept all responsibility for their implementation and use.

## 🎯 The Four Pillars + Safety First

### SAFETY FIRST: Physical Wired Control Required

⚠️ **CRITICAL FOR VEHICLE SAFETY**

Any device that controls vehicle critical functions (motors, fans, lights, levelers, heaters, etc.) **MUST have a reliable physical wired control option**:

- ✅ **CAN Bus Control**: Primary reliable wired control via CAN
- ✅ **Physical Controls**: Hardwired switches, buttons, or panels with no wireless dependency
- ✅ **No Single Points of Failure**: Users can always control critical devices even if WiFi/Cloud fails
- ✅ **WiFi as Optional Enhancement**: Users can add WiFi control IF they choose, but it's never the only option

**What This Means:**
- Critical device control is hardwired first
- WiFi/Cloud control is optional enhancement
- Physical panel or CAN interface always available
- Vehicle never depends on WiFi for critical operations

**What This Prevents:**
- ❌ WiFi-only control of critical devices
- ❌ Dependence on wireless for motor/fan/light control
- ❌ Inability to operate without network connectivity
- ❌ Vehicle systems failing due to WiFi interference or disconnection

**Examples:**

✅ CORRECT - Heater Control:
- Primary: Physical on/off switch hardwired to relay
- Secondary: CAN bus control from in-vehicle compute
- Optional: Cloud remote control (if user wants)
- User can always turn heater on/off manually even if everything else fails

❌ WRONG - Heater Control:
- WiFi-only control
- No physical switch
- Must be connected to network to use
- Failure point if WiFi fails

---

### AUTONOMOUS INTELLIGENCE: Local Decision-Making and Action

🧠 **LOCAL AUTONOMOUS CAPABILITIES**

TrailCurrent includes autonomous intelligence components that run locally on the vehicle's edge compute system, making real-time decisions and taking actions without cloud or internet dependency:

- ✅ **Local AI/ML Components**: Machine learning models and decision logic run entirely on-vehicle
- ✅ **Real-Time Autonomous Actions**: Vehicle responds intelligently to conditions, patterns, and events
- ✅ **Offline-Capable Intelligence**: All autonomous features work without internet or cloud
- ✅ **User-Controlled Algorithms**: Users understand and can modify what intelligence runs on their vehicle
- ✅ **No Cloud AI Dependency**: Intelligence never requires external services

**What This Means:**
- Vehicle can learn patterns and adapt behavior autonomously
- Intelligent responses to detected conditions happen immediately, locally
- Decision-making algorithms are transparent and open source
- Users control what autonomous behaviors are enabled
- Intelligence improves vehicle efficiency and safety proactively

**What This Prevents:**
- ❌ Dependence on cloud AI services
- ❌ Black-box decision-making systems users cannot understand
- ❌ Intelligence that requires constant internet connection
- ❌ Proprietary machine learning models
- ❌ Autonomous features that fail without cloud connectivity

**Examples of Autonomous Intelligence (Concepts):**
- Proactively managing system resources based on usage patterns and objectives
- Automatically responding when safety conditions are detected
- Adapting system configuration based on environmental conditions
- Learning from historical patterns to optimize system behavior
- Making intelligent decisions about system prioritization

All autonomous features remain under user control - users decide what intelligence is active, can modify algorithms, and can always override with physical controls.

---

## 🎯 The Three Pillars

### 1. ⚪ Cloud is Optional

**The System is Designed for Complete Autonomy**

- ✅ Vehicles operate with **full functionality** without any cloud connection
- ✅ In-vehicle compute system is self-contained and needs **no external services**
- ✅ All critical features work **offline** using local MQTT messaging
- ✅ Cloud is purely **optional** for enhanced monitoring and remote management
- ✅ Users should **never be forced** to use cloud services
- ✅ If cloud features are used, it's **entirely the user's choice**

**What This Means:**
- No feature can depend on cloud connectivity
- Test everything in offline mode
- Local MQTT messaging is the default
- Cloud is a nice-to-have, not a requirement

**What This Prevents:**
- ❌ Required cloud authentication to use vehicle
- ❌ Features that are cloud-only
- ❌ Systems that degrade significantly offline
- ❌ Vendor lock-in to any cloud platform

---

### 2. 🔐 Data Privacy First

**All Vehicle Data Stays Private and Under User Control**

- ✅ **No data leaves the vehicle** unless the user explicitly chooses to send it
- ✅ **No third-party APIs** required or integrated (AWS, Google, Azure, etc.)
- ✅ **No tracking or telemetry** - complete user control of data
- ✅ **No vendor lock-in** - use open standards and self-hostable solutions
- ✅ **Users own their data** - they host it, they control it
- ✅ **Transparent data flows** - all communication is documented and auditable

**What This Means:**
- Sensors and modules collect data locally
- Data stays on the vehicle until sent to user's chosen location
- No external analytics, tracking, or monitoring
- Users can self-host cloud (if used) on their own servers

**What This Prevents:**
- ❌ Sending data to cloud by default
- ❌ Using Big Tech APIs (AWS, Google Cloud, Azure, etc.)
- ❌ Integrated telemetry or crash reporting services
- ❌ Subscription models that track usage
- ❌ Data harvesting or third-party integrations
- ❌ Regional data restrictions or GDPR workarounds

**Example - Correct Approach:**
User can self-host PostgreSQL on their NAS, then send data there from vehicle. Completely private.

**Example - Wrong Approach:**
Using AWS DynamoDB, Firebase, or Twilio SMS - these are Big Tech services that violate this principle.

---

### 3. 📖 100% Open Source

**Everything is Open, Transparent, and Community-Driven**

- ✅ **All code is open source** - firmware, backend, frontend, tools
- ✅ **All designs are open** - CAD files, EDA schematics, PCB layouts
- ✅ **All dependencies are open source** - no proprietary components
- ✅ **All tools are open source** - use KiCad (not Altium), FreeCAD (not SolidWorks)
- ✅ **No license lock-in** - GPL, MIT, Apache, etc. but never proprietary
- ✅ **Community contributions welcome** - transparent development process

**What This Means:**
- Anyone can view, modify, and redistribute the entire system
- No black-box components or closed-source libraries
- Tools and design files can be opened with free/open software
- No license keys, subscriptions, or paywalls for core functionality

**What This Prevents:**
- ❌ Proprietary components or black-box systems
- ❌ Closed-source dependencies or libraries
- ❌ Vendor-specific tools (Altium Designer, SolidWorks, etc.)
- ❌ License lock-in or subscription requirements
- ❌ Trade secrets or intentional obfuscation

**Example - Correct Approach:**
Using KiCad for PCB design, with all schematics in version control. Anyone can fork and modify.

**Example - Wrong Approach:**
Using Altium Designer for schematics (proprietary tool, expensive), or closed-source firmware.

---

## 🚨 Red Flags for Violations

If any of these happen, **stop and discuss in an issue first**:

### Cloud Dependency Red Flags
- "You need to create an account to use this feature"
- "This only works if you connect to our cloud"
- "Offline mode is degraded/limited"
- "Critical features require internet"

### Data Privacy Red Flags
- "Let's integrate with AWS/Google/Azure"
- "Let's send sensor data to a third-party service"
- "Let's add telemetry/analytics"
- "Let's use a SaaS platform for storage"
- "Let's integrate Twilio/SendGrid/etc."

### Open Source Red Flags
- "Let's use this proprietary tool"
- "Let's add a closed-source library"
- "Let's use a subscription SaaS tool"
- "Let's integrate with a commercial service"
- "CAD files are .rar archives, not source"

---

## 🛠️ Making Decisions

**When adding a feature, ask:**

1. **Cloud Dependency Question**
   - Does this feature require cloud? → NO
   - If it helps with cloud, is it optional? → YES
   - Does it work offline? → YES

2. **Data Privacy Question**
   - Does this send data externally? → ONLY if user controls it
   - Is any third-party API used? → ONLY if self-hostable alternative exists
   - Can user see where data goes? → YES

3. **Open Source Question**
   - Is all code open source? → YES
   - Are all dependencies open source? → YES
   - Can CAD/designs be edited without proprietary tools? → YES

**If any answer conflicts with these principles, redesign or don't add the feature.**

---

## 💡 Example Decisions

### ✅ GOOD: Adding GPS Tracking

- Hardware: GPS module sends coordinates via CAN ✓
- Processing: In-vehicle compute stores locally ✓
- Optional Cloud: User can self-host PostGIS on own server ✓
- Privacy: All data stays on user's infrastructure ✓
- Open Source: KiCad design, C firmware, Node.js backend ✓

### ❌ BAD: "Cloud-Based Vehicle Monitoring"

- Requires: AWS or Google Cloud ✗ (Big Tech)
- Data: Automatically sent to company servers ✗ (Not private)
- Control: User can't self-host ✗ (Vendor lock-in)
- Offline: Limited functionality without internet ✗ (Cloud dependency)

### ✅ GOOD: Adding Remote Diagnostics

- Local: In-vehicle compute logs all data ✓
- Optional: User can pull logs via local MQTT ✓
- Self-Hosted: Can run TLS tunnel to own VPS ✓
- Privacy: User chooses if/when to access ✓
- Open: All code and protocols documented ✓

### ❌ BAD: "Integration with Twilio SMS"

- External Service: Twilio is third-party ✗ (Not private)
- Cost: Subscription required ✗ (Vendor lock-in)
- Control: Can't self-host SMS gateway ✗ (Proprietary)
- Privacy: Phone numbers sent to Twilio ✗ (Data leak)

---

## 📋 For Developers

**Before Contributing:**
1. Read these principles
2. Understand the intent, not just the rules
3. When in doubt, discuss in an issue
4. Test offline functionality
5. Check for data privacy implications

**During Code Review:**
- Reviewers will check all three principles
- Features violating these will be rejected
- PRs should explicitly address how they maintain these principles

**The Goal:**
To build a system that is:
- **Independent** - Works without cloud
- **Private** - User controls all data
- **Open** - Transparent and modifiable

---

## 🤝 Questions?

If you're unsure whether something aligns with these principles:

- **Open an issue** - Discuss the design first
- **Don't guess** - Ask the maintainers
- **Collaborate** - Find a solution that respects all three principles

Remember: **These principles are not restrictions, they're the foundation that makes TrailCurrent special.**

---

## Related Documents

- [CONTRIBUTING.md](07_Development/CONTRIBUTING.md) - How to contribute while maintaining principles
- [01_Architecture/PLATFORM_OVERVIEW.md](01_Architecture/PLATFORM_OVERVIEW.md) - System architecture
- [README.md](README.md) - Project overview
