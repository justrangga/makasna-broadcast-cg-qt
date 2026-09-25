<div align="center">
  <img src="resources/makasna-logo.svg" width="220" alt="MAKASNA LOGO" />
  <h1>Makasna Broadcast Character Generator</h1>
  <p><strong>Native C++20 &amp; Qt6 Real-Time Broadcast Graphics Engine</strong></p>
</div>

---

## Overview

Makasna Broadcast CG is a high-performance broadcast character generator and real-time graphics playout system engineered for live television production, OB vans, and master control environments.

Built on native C++20 and Qt6 (QML), Makasna Broadcast CG operates as a single, lightweight native process in Windows without Chromium or Electron runtimes. The architecture delivers deterministic 60 fps GPU compositing via Direct3D 11 and OpenGL RHI with microsecond-level clock accuracy, minimal memory overhead (~40 MB baseline RAM), and zero garbage collection latency.

---

## Core Capabilities

### Broadcast Video I/O and SDI Playout
- **Blackmagic Design DeckLink Integration:** Native Windows COM implementation (`IDeckLinkIterator`, `IDeckLink`) with automatic physical device enumeration across PCIe and Thunderbolt hardware.
- **External Key & Fill Playout:** Dedicated routing separating SDI 1 (Color Fill) and SDI 2 (Linear Alpha Key Matte) for production switchers (Blackmagic ATEM, Ross Carbonite, Grass Valley).
- **Secondary Display Output:** Borderless transparent fullscreen renderer targeting secondary HDMI / DisplayPort monitors.
- **Broadcast Standards Support:** 1080p60, 1080p59.94, 1080p50, 1080i59.94, 1080i50, 720p60, and 2160p (4K UHD).

### Smart Data Hub and Automation
- **Local File Live Watcher:** Watches local Excel (.xlsx, .csv) and JSON files via `QFileSystemWatcher`. Saving file updates in external editors triggers instant on-air graphic reloads without application restart.
- **Google Sheets Live Sync:** Direct polling synchronization with published Google Spreadsheets at configurable intervals (1 to 60 seconds).
- **REST JSON and XML Feeds:** Automated HTTP polling client for online data APIs, sports scoreboards, and RSS news tickers.
- **RossTalk Protocol Listener:** Embedded TCP listener on port 7788 for production switcher macro triggers (`TAKE`, `CUE`, `CLEAR`, `GOAL`).
- **HTTP Playout Server:** Embedded server on port 4989 providing canvas access for IP-based workflows.

### Dual Operating Workspaces
- **Designer Studio:** 1080p WYSIWYG canvas, SMPTE Safe Zone overlays (Action Safe 90%, Title Safe 80%), interactive layer dragging with automatic animation delta synchronization, axis lock, center snapping, and multi-track keyframe timeline ruler.
- **Playout Console:** Dual-tally Preview (Green) and Program (Red) monitors, Rundown Playlist management, layer clear triggers (L1 to L4), scorebug controls, and digital match clock.

---

## System Requirements

- **Operating System:** Windows 10 / Windows 11 (64-bit)
- **Processor:** Intel Core i5 / AMD Ryzen 5 or higher
- **Memory:** 4 GB RAM minimum (application footprint is under 60 MB)
- **Graphics:** Direct3D 11 or OpenGL 4.5 capable GPU
- **Broadcast Hardware (Optional):** Blackmagic Design DeckLink (Duo 2, Quad 2, 8K Pro, Mini Monitor 4K, UltraStudio) with Desktop Video 12.0 or later

---

## Download and Deployment

### Pre-Built Binaries
Ready-to-use release packages are published on the GitHub Releases page:
- Repository: [justrangga/makasna-broadcast-cg-qt](https://github.com/justrangga/makasna-broadcast-cg-qt)
- Download `Makasna.Broadcast.CG.Native-Qt6-Windows.zip`, extract to any folder, and execute `MakasnaBroadcastCG.exe`.
- All required runtime dependencies and Qt6 libraries are packaged via `windeployqt`. No external runtime installations are required.

### Building from Source

```bash
# Clone the repository
git clone https://github.com/justrangga/makasna-broadcast-cg-qt.git
cd makasna-broadcast-cg-qt

# Configure with CMake
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# Compile the target
cmake --build build --config Release --parallel
```

Requirements for source build:
- CMake 3.24 or higher
- C++20 compliant compiler (MSVC 2019/2022 on Windows, GCC 11+ or Clang 13+ on Linux)
- Qt 6.7+ (Core, Gui, Quick, Qml, Network, Svg, QuickControls2)

---

## License

Proprietary - MAKASNA Broadcast Technologies. All rights reserved.
