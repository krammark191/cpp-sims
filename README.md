# C++ Simulators

Native C++ sampler application that combines four legacy course simulators into a single macOS app:

- Apollo 11 Lander
- Artillery Howitzer
- Chess
- Orbital Simulator

The merged app keeps the original OpenGL/GLUT-style engine approach while adding a shared shell, common menu flow, per-simulator adapters, preview assets, packaging scripts, and test coverage.

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

## Generated Output

These paths are generated locally and are ignored by Git:

- `build/`
- `dist/`

## Notes for GitHub

- The working folder is ready to initialize as a Git repository.
- Xcode now sees the full source tree in the project navigator.
- The Xcode target has been narrowed so it builds the windowed app entry point instead of compiling both console and window mains.
