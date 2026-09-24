# 📺 Makasna Broadcast CG (Native C++20 & Qt6)
*Standar Software Character Generator (CG) Televisi Broadcast Tier-1 Tanpa Chromium / Zero-Electron*

---

## ⚡ Keunggulan Arsitektur Native C++20 & Qt6:
- **Zero-Chromium / Zero-Electron:** Menghapus beban browser Chromium. Berjalan sebagai *single process* murni di Windows Task Manager.
- **Konsumsi RAM Sangat Rendah:** Baseline hanya **~40–70 MB RAM** (dibandingkan Electron yang memakan 350–500 MB).
- **Akselerasi GPU Murni (Direct3D 11 / OpenGL RHI):** Rendering 60fps tanpa jeda pembersihan memori (*zero garbage collection pause*).
- **Integrasi Hardware Penyiaran TV:**
  - **Blackmagic Design DeckLink SDK:** Native SDI Key & Fill (SDI 1 = Fill, SDI 2 = Alpha Matte) untuk switcher video broadcast (ATEM Constellation, Ross Carbonite).
  - **Secondary Display Output:** Output fullscreen borderless dengan latar transparan murni untuk HDMI / DisplayPort.
  - **RossTalk TCP Automation:** Port `7788` untuk menerima perintah otomatis dari video switcher dan Bitfocus Companion (`TAKE`, `CUE`, `CLEAR`, `GOAL`).
  - **Embedded Web Canvas Server:** Port `4989` untuk fallback ingest browser source OBS / vMix.
- **UI Workspace Dual-Mode:**
  - 🎨 **Designer Studio:** Kanvas WYSIWYG 1080p, panduan Safe Zone SMPTE (Action 90%, Title 80%), layer drag-and-drop, timeline keyframe dengan smooth mouse scrubbing.
  - ⚡ **Playout Console:** Dual monitor tally (Preview hijau / Program merah), Rundown playlist, dan kontrol cepat score/match clock.

---

## 📥 Download Windows Executable:
Download paket rilis Windows portable `.zip` langsung dari [GitHub Releases](https://github.com/justrangga/makasna-broadcast-cg-qt/releases).
Ekstrak file `.zip` dan jalankan `MakasnaBroadcastCG.exe`.
