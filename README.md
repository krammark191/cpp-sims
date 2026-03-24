# C++ Simulators

Native C++ sampler application that combines four legacy course simulators into a single desktop release project:

- Apollo 11 Lander
- Artillery Howitzer
- Chess
- Orbital Simulator

The merged app keeps the original OpenGL/GLUT-style engine approach while adding a shared shell, common menu flow, per-simulator adapters, preview assets, packaging scripts, release automation, and test coverage.

## Visual Overview

App icon:

![C++ Simulators icon](assets/icon/app-icon-source.png)

Preview keyframes:

| Apollo 11 Lander | Artillery Howitzer |
| --- | --- |
| ![Apollo 11 Lander preview](assets/readme/apollo11_lander_keyframe.png) | ![Howitzer preview](assets/readme/howitzer_keyframe.png) |
| Chess | Orbital Simulator |
| ![Chess preview](assets/readme/chess_keyframe.png) | ![Orbital preview](assets/readme/orbital_keyframe.png) |

## Downloads

Published binaries should be distributed through GitHub Releases:

- Latest release page:
  - [https://github.com/krammark191/cpp-sims/releases/latest](https://github.com/krammark191/cpp-sims/releases/latest)
- Latest macOS dmg:
  - [https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators.dmg](https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators.dmg)
- Windows package zip:
  - [https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators-windows.zip](https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators-windows.zip)
- Raw Windows exe asset:
  - [https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators.exe](https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators.exe)

These direct asset links resolve to the most recent published release. On Windows, the zip package is the intended user download because it includes the executable together with the required assets and runtime DLLs.

## Repository Layout

- `C++ Simulators/`
  - desktop entry points used by the Xcode target and shared launcher sources
- `app/`
  - shared application shell, runtime controller, menu/presenter logic, preview handling
- `engine/`
  - engine-side rendering helpers, image-sequence loading, shell rendering
- `simulators/`
  - simulator-specific scene adapters plus imported legacy source trees
- `tests/`
  - unit tests for the merged shell and simulator adapters
- `assets/`
  - preview manifests, images, and imported effect assets
- `scripts/`
  - local packaging and release publish scripts
- `docs/`
  - merge planning, cleanup notes, packaging, and release documentation

## Build Paths

### Xcode

Open:

- `C++ Simulators.xcodeproj`

The `C++ Simulators` target is configured to build the desktop window app entry point from:

- `C++ Simulators/window_main.cpp`

### Command Line

Primary project build definitions live in:

- `CMakeLists.txt`

Packaging scripts:

- `scripts/package_macos_app.sh`
- `scripts/package_macos_dmg.sh`
- `scripts/publish_github_release.sh`

GitHub Actions workflow:

- `.github/workflows/windows-release.yml`

## Generated Output

These paths are generated locally and are ignored by Git:

- `build/`
- `dist/`

## GitHub Release Flow

- Build and package the macOS deliverable into `dist/C++ Simulators.dmg`
- Publish a release tag with `scripts/publish_github_release.sh <tag>`
- Let the Windows GitHub Actions workflow build and attach:
  - `C++ Simulators-windows.zip`
  - `C++ Simulators.exe`

See:

- `docs/github-releases.md`
- `docs/release-checklist.md`
- `CHANGELOG.md`

## Notes for GitHub

- Xcode now sees the full source tree in the project navigator.
- The Xcode target has been narrowed so it builds the windowed app entry point instead of compiling both console and window mains.
