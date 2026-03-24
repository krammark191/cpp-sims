# C++ Simulators Merge Plan

## Goal

Combine the four existing C++ projects into one sampler application that keeps the current engine style, adds a menu shell, and reduces duplicated code without breaking simulator behavior.

## Decisions Locked In

- Keep the current visual and engine style for the first merged version.
- Target macOS first.
- Use prerecorded looping video demos on the Play screen.
- Add audio settings hooks now, but defer real audio implementation until assets exist.
- Preserve local two-player Chess now, while leaving architecture room for AI and puzzle modes later.
- Audit and correct Chess rules logic so it aligns with official chess rules before treating the merged version as feature-complete.
- Use clearer type names at the shared-engine boundary, such as `Position2D` or `WorldPosition`, instead of forcing Chess and physics code to share one `Position` type.

## Source Projects Located

- Apollo 11 Lander: `/Users/krammark191/Documents/BYUI Files/CSE 230/Encapsulation-Design-Assignments/Lab 06 Apollo 11 Final`
- Howitzer: `/Users/krammark191/Documents/BYUI Files/CSE 230/Encapsulation-Design-Assignments/Lab 12 Artillery Howitzer, Projectile`
- Chess: `/Users/krammark191/Documents/BYUI Files/CSE 231/Inheritance-Design-Assignments/W06 Chess Final`
- Orbital Simulator: `/Users/krammark191/Documents/BYUI Files/CSE 231/Inheritance-Design-Assignments/W08 Lab Orbit One Satellite`

## Current Findings

### Workspace state

- The current workspace contains an Xcode project and a stub entry point at `C++ Simulators/main.cpp`.
- No merged source tree exists yet, so this workspace is a clean target for consolidation.

### Natural project families

- Apollo and Howitzer share an early physics/input/render stack.
- Howitzer and Orbital share the strongest overlap in physics-domain files.
- Chess shares the OpenGL-style windowing approach, but its `Position` and input model are structurally different from the physics simulators.

### Duplicate files are not all equivalent

- `acceleration.cpp` is identical between Howitzer and Orbital.
- `test.h` is identical between Howitzer and Orbital.
- `unitTest.h` is identical between Howitzer and Chess.
- Many same-name files have drifted APIs and cannot be merged blindly.
- Chess's `position.h` is a board-coordinate type and should not be forced into the same class as the physics `Position`.

## Standardization Plan

### Tier 1: Shared platform layer

Create a small common application shell used by all four simulators:

- `app/Application`
- `app/AppState`
- `app/MenuState`
- `app/OptionsState`
- `app/AboutState`
- `app/PauseOverlay`
- `app/InputState`
- `app/ControlHintOverlay`
- `app/SimulatorRegistry`

Responsibilities:

- startup and main menu
- switching between menu and simulator scenes
- ESC pause behavior
- five-second control overlay timer
- shared sound/settings storage
- shared About data
- playback control for prerecorded preview loops in the Play screen

This layer should be new code. It should wrap the existing engine rather than rewriting simulator logic immediately.

### Tier 2: Shared engine modules

Unify only the files that already represent the same domain model.

Good first candidates:

- `physics/Acceleration`
- `physics/Velocity`
- `physics/Angle`
- `physics/Position2D`
- `physics/PhysicsCommon`
- `render/StarField`
- `engine/FrameTimer`

Important rule:

- Use new names where necessary instead of forcing incompatible classes into one filename.
- Example: keep Chess on `BoardPosition` or `ChessPosition` rather than reusing physics `Position`.

### Tier 3: Simulator adapters

Each simulator should be wrapped behind a common interface:

- `ISimulatorScene`
- `LanderScene`
- `HowitzerScene`
- `ChessScene`
- `OrbitalScene`

Suggested interface:

- `enter()`
- `exit()`
- `update(double dt, const InputState&)`
- `render(ogstream&)`
- `reset()`
- `getMetadata()`
- `getKeybindings()`
- `supportsDemoMode()`
- `runDemoStep(double dt)`

This allows the shell to treat each simulator uniformly while preserving internal differences.

## Demo Strategy

For the first milestone, prerecorded looping demos are the better option.

Reasons:

- faster and more predictable than spinning up four live simulations in the selection screen
- avoids async loading and simulator-state coordination problems in the menu shell
- keeps menu performance stable on the existing GLUT/OpenGL stack
- lets each simulator have a polished preview without sharing runtime state with the live playable scene

Follow-up requirement:

- keep the preview player isolated from simulator runtime so a selected simulator always launches from a clean reset state

## File Consolidation Recommendations

### Merge soon

- Howitzer and Orbital `acceleration.*`
- Howitzer and Orbital `test.h`
- Shared unit-test harness patterns
- Shared menu/input/timing code added in the new app shell

### Merge after API cleanup

- Apollo/Howitzer/Orbital `velocity.*`
- Apollo/Howitzer/Orbital `angle.*`
- Howitzer/Orbital `position.*`
- Apollo/Howitzer/Orbital `uiInteract.*` into one richer interface

### Do not merge directly

- Chess `position.*` with physics `position.*`
- Chess `uiDraw.*` with physics/orbital render code
- Chess mouse-selection input with simulator arrow/thrust input

Instead, standardize those through adapters and shared shell contracts.

Naming guidance:

- canonical shared physics type: `Position2D` or `WorldPosition`
- canonical chess type: `BoardPosition` or `ChessPosition`

## Proposed Target Layout

```text
C++ Simulators/
  CMakeLists.txt
  app/
  engine/
  render/
  physics/
  simulators/
    lander/
    howitzer/
    chess/
    orbital/
  assets/
    audio/
    docs/
    metadata/
  tests/
    unit/
    integration/
    golden/
```

## Merge Phases

### Phase 1: Import and build isolation

- Copy each simulator into `simulators/<name>/legacy/` or equivalent source folders.
- Make each one compile inside the unified repo before refactoring.
- Keep original filenames during import to reduce risk.

### Phase 2: Build system unification

- Replace per-project Xcode/Visual Studio files with one CMake build.
- Produce:
  - macOS `.app` bundle for local use
  - macOS `.dmg` for distribution later
- A `.jar` is not appropriate here because this is a native C++ application.

Windows packaging is deferred until after the first macOS milestone.

### Phase 3: App shell

- Implement main menu, play menu, options, about, quit flow.
- Add simulator preview playback for prerecorded looping demos on the play screen.
- Add shared pause overlay on ESC.
- Add control hints that auto-hide after five seconds and can be reopened.

### Phase 4: Shared engine extraction

- Move compatible physics classes into shared modules.
- Refactor simulator code to include shared headers.
- Add conversion wrappers where API drift exists.

### Phase 5: Cleanup

- Remove dead duplicate files once all simulators use shared modules.
- Replace embedded author/assignment comments with app metadata and About content.

## Options Menu Suggestions

- master volume
- music volume
- effects volume
- mute toggle
- show controls on start
- control hint duration
- windowed/fullscreen
- resolution
- frame rate cap
- visual scale or zoom preset
- difficulty preset for demos only
- reset progress/settings

## About Screen Suggestions

- team/developer credits
- short history of the course sequence
- simulator summary cards
- core controls per simulator
- technical notes: OpenGL/GLUT, physics model, inheritance/polymorphism highlights
- acknowledgements for instructor-provided engine code if desired

## Demo/Selection Screen Suggestions

- Four looping preview tiles, one per simulator
- Hover or keyboard focus shows summary and controls
- Press Enter or click to launch selected simulator
- Preview playback should loop cleanly and not share live runtime state with the playable simulator

## Testing Plan

### Preserve and import existing tests

- Keep original per-simulator unit tests during the import phase.
- Compile them into separate test targets so regressions are visible before deduplication.

### Add new unit tests

- `tests/unit/app_state_tests.cpp`
  - startup enters main menu
  - menu transitions route correctly
  - quit action triggers shutdown state
- `tests/unit/pause_overlay_tests.cpp`
  - ESC opens pause state
  - resume returns to simulator
  - keybindings panel opens from pause menu
- `tests/unit/control_hint_tests.cpp`
  - controls visible for first five seconds
  - controls hide after timeout
  - controls can be shown again from pause menu
- `tests/unit/simulator_registry_tests.cpp`
  - all four simulators register
  - metadata and keybindings exist for all entries

### Add integration tests

- launch app -> menu appears
- choose Play -> simulator gallery appears
- start each simulator from gallery
- ESC pauses each simulator
- Quit from pause returns to menu
- Quit to Desktop exits cleanly

### Add regression tests for shared modules

- physics math still matches legacy expectations
- board coordinate logic in Chess remains unchanged
- chess rules logic matches official movement and game-state rules
- orbital collision behavior remains stable for fixed seeds

### Add chess rules audit coverage

- legal movement for every piece
- capture rules
- blocked-path validation for sliding pieces
- check detection
- illegal self-check prevention
- checkmate and stalemate detection
- castling rules
- en passant
- pawn promotion
- move-history-dependent state needed for later AI/puzzle work

## Technical Notes and Risks

- The current engine is GLUT/OpenGL based. That is workable, but the app shell will likely require a richer input abstraction than the current project-specific `Interface` classes.
- The biggest naming conflict is `Position`.
- The biggest integration risk is trying to unify render and input code too early.
- Orbital currently has the most evolved feature set and is a good source for the final shared physics APIs.
- Chess should probably keep its own board render/input path while conforming to the common scene interface.
- Looping prerecorded demos reduce menu complexity, but they introduce asset-pipeline work and playback integration that should stay separate from live simulation startup.

## Immediate Next Steps

1. Import all four codebases into this repo without behavior changes.
2. Set up a unified CMake build with one executable and separate simulator/static-library targets.
3. Define the shared `ISimulatorScene` and `AppState` interfaces.
4. Choose canonical shared physics classes, starting with Howitzer/Orbital.
5. Add new tests for menu state, pause state, control overlays, and preview playback control before implementing UI shell behavior.
6. Run a focused audit on Chess rules correctness and log all gaps against official rules before any shared refactor touches Chess behavior.
