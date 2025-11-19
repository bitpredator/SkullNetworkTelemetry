# SkullNetworkTelemetry

## Overview
**SkullNetworkTelemetry** is a custom telemetry plugin for Euro Truck Simulator 2 (ETS2) and American Truck Simulator (ATS), designed specifically for the Skull Network Italia community. Unlike standard mods, this plugin is implemented as a standalone **DLL** (`SkullNetworkTelemetry.dll`) that interfaces directly with the SCS Telemetry API and transmits vehicle and job data to a web-based map system.

The current version provides the foundational structure for a telemetry system, including:

- DLL entry points compatible with SCS Telemetry API (`scs_plugin_init` and `scs_plugin_shutdown`)
- Background worker thread for continuous telemetry updates
- TCP network client for sending JSON payloads to a backend server
- Minimal telemetry payload structure with session ID, vehicle position, speed, heading, and truck information
- Modular and scalable C++17 codebase

This project is developed using **Visual Studio Code** and targets **x64 Windows systems**, ensuring compatibility with the latest ETS2 and ATS versions.

---

## Current Status
- Initial DLL skeleton implemented (`plugin.h`, `plugin.cpp`, `dllmain.cpp`)
- Thread-safe network client implemented for reliable TCP communication
- Telemetry payloads serialized to JSON and sent at configurable intervals
- Placeholder data provided for initial testing; ready for integration with real SCS API callbacks

---

## Future Work
The following features are planned for the next development phases:

1. **Full SCS Telemetry API Integration**
   - Register callbacks for truck data, trailer, job, and engine parameters
   - Extract real-time position, speed, damage, and cargo information
2. **Extended Payload**
   - Include departure and destination cities
   - Include cargo, job status, and additional vehicle metrics
   - Add configurable fields for community-specific telemetry
3. **Backend Integration**
   - Build a C# server capable of receiving and storing telemetry data
   - Implement authentication and secure data transmission
   - Provide live map updates on the Skull Network Italia website
4. **Performance and Reliability**
   - Implement batching or compression of telemetry data (JSON/Gzip/Protobuf)
   - Ensure minimal CPU and memory impact on the game
5. **Configuration and Deployment**
   - Configurable host/port via JSON file
   - Easy deployment as a DLL plugin for ETS2/ATS without modifying the game

---

## Requirements
- Windows 10/11 x64
- ETS2 or ATS (x64)
- Visual Studio Code (or Visual Studio with C++ build tools)
- Windows SDK installed (for headers like `windows.h`, `winsock2.h`)

---

## Usage
1. Compile the project as **x64 DLL**.
2. Copy `SkullNetworkTelemetry.dll` to the ETS2/ATS plugins folder:


3. Launch the game; the plugin initializes automatically and begins sending telemetry data.
4. Connect your backend server to the default TCP port (5500) or configure via JSON.

---

## License
This project is **open-source** and intended for the Skull Network Italia community. Contributions and suggestions are welcome.

---

## Contributing
Please open GitHub issues or pull requests for bug fixes, feature requests, or improvements. Ensure all contributions maintain thread safety and compatibility with x64 ETS2/ATS builds.

