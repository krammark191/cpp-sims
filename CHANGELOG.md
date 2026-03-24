# Changelog

All notable release-facing changes for this project are tracked here.

## v0.1.5

Release date: 2026-03-24

- Added a working Windows release path through GitHub Actions, including:
  - `C++ Simulators-windows.zip`
  - `C++ Simulators.exe`
- Fixed Windows portability issues in the desktop window target:
  - added the required Chess OpenGL include path on Windows
  - linked GLU for the legacy interaction layer
  - preserved the existing macOS build path
- Kept macOS packaging and Releases active through:
  - `C++ Simulators.dmg`
  - `scripts/package_macos_dmg.sh`
  - `scripts/publish_github_release.sh`
- Refreshed repo and in-app project copy so README, release docs, and About text match the current cross-platform release state.
