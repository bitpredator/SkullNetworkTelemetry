# SkullNetworkTelemetry

## Overview
**SkullNetworkTelemetry** is a custom telemetry plugin for *Euro Truck Simulator 2* (ETS2) and *American Truck Simulator* (ATS), built specifically for the **Skull Network Italia** community.

The plugin operates as a standalone **DLL** (`SkullNetworkTelemetry.dll`) placed in the game’s `/plugins` folder.  
It communicates directly with the **SCS Telemetry API** and sends structured telemetry data to a backend server for live map visualization and community features.

---

## Features (Current)
- ✔ Correctly exported SCS API plugin entry points  
- ✔ Background telemetry thread via `TelemetryManager`  
- ✔ TCP client (`NetworkClient`) that sends JSON payloads periodically  
- ✔ C++20 clean architecture  
- ✔ Full DLL stability (no more missing symbols or export issues)  
- ✔ Ready for integration with real truck/job telemetry callbacks  

---

## Latest Changes
See the latest changelog in the pull request for all updates related to:
- SCS SDK headers
- Exported functions
- CMake improvements
- Network and telemetry loop stabilization
- Proper shutdown handling

---

## Future Work
Planned features include:

1. **Full Telemetry Integration**
   - Position, speed, orientation
   - Trailer & job data
   - Cargo info, damage, economy stats

2. **Extended JSON Payload**
   - Origin/destination cities
   - Job progress
   - Configurable telemetry fields

3. **Backend Integration**
   - Official Skull Network Italia back-end server  
   - Secure communication  
   - Live map support  

4. **Performance Improvements**
   - Efficient serialization  
   - Optional GZip/Protobuf  
   - Optimized threading  

5. **Config System**
   - Custom `config.json` (host/port/interval, etc.)

---

## Requirements
- Windows 10/11 x64  
- ETS2 or ATS (x64)  
- Visual Studio Build Tools / MSVC  
- Windows SDK (for `windows.h`, Winsock)  
- CMake or Visual Studio  

---

## Build & Installation
1. Compile as **x64 DLL**.
2. Copy `SkullNetworkTelemetry.dll` to:

<ETS2 OR ATS INSTALL FOLDER>/bin/win_x64/plugins/


3. Launch the game.  
4. The plugin automatically initializes and connects to the backend (default port: **5500**).

---

## Community & Support
Join the official **Skull Network Italia Discord**:  
👉 **https://discord.gg/Jrm2Z26ad3**

---

## License
This project is open-source and dedicated to the **Skull Network Italia** community.

---

## Contributing
Pull requests are welcome!  
Please ensure code is:
- Thread-safe  
- Consistent with modern C++ guidelines  
- Compatible with ETS2/ATS x64  
