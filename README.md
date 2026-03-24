# C++ Simulators

Native C++ sampler application that combines four legacy course simulators into a single macOS app:

- Apollo 11 Lander
- Artillery Howitzer
- Chess
- Orbital Simulator

The merged app keeps the original OpenGL/GLUT-style engine approach while adding a shared shell, common menu flow, per-simulator adapters, preview assets, packaging scripts, and test coverage.

## Downloads

Published binaries should be distributed through GitHub Releases:

- Latest release page:
  - [https://github.com/krammark191/cpp-sims/releases/latest](https://github.com/krammark191/cpp-sims/releases/latest)
- Latest macOS dmg:
  - [https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators.dmg](https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators.dmg)
- Future Windows exe:
  - [https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators.exe](https://github.com/krammark191/cpp-sims/releases/latest/download/C%2B%2B%20Simulators.exe)

These direct asset links start working once a GitHub release is published with matching asset names.

## Repository Layout

- `C++ Simulators/`
  - macOS entry points used by the Xcode target
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
  - macOS app and dmg packaging scripts
- `docs/`
  - merge planning, cleanup notes, and packaging documentation

## Build Paths

### Xcode

Open:

- `C++ Simulators.xcodeproj`

The `C++ Simulators` target is configured to build the macOS window app entry point from:

- `C++ Simulators/window_main.cpp`

### Command Line

Primary project build definitions live in:

- `CMakeLists.txt`

Packaging scripts:

- `scripts/package_macos_app.sh`
- `scripts/package_macos_dmg.sh`
- `scripts/publish_github_release.sh`

## Generated Output

These paths are generated locally and are ignored by Git:

- `build/`
- `dist/`

## GitHub Release Flow

- Build and package the macOS deliverable into `dist/C++ Simulators.dmg`
- Optionally stage a Windows build as `dist/C++ Simulators.exe`
- Publish the release with:
  - `scripts/publish_github_release.sh <tag>`

See:

- `docs/github-releases.md`

## Notes for GitHub

- Xcode now sees the full source tree in the project navigator.
- The Xcode target has been narrowed so it builds the windowed app entry point instead of compiling both console and window mains.
