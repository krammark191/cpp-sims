# Shared-File Cleanup

This is the first staged cleanup pass for the merged simulator app. The current rule is:
keep legacy simulator code stable where it protects behavior, and extract shared code only from
the merged shell/render layer until the scene adapters are fully proven.

## Completed

- Extracted shared scene render helpers into:
  - `simulators/common/scene_render_utils.h`
  - `simulators/common/scene_render_utils.cpp`
- Moved duplicated panel/marker/outline drawing out of:
  - `simulators/chess/chess_scene.cpp`
  - `simulators/howitzer/howitzer_scene.cpp`
  - `simulators/orbital/orbital_scene.cpp`
- Added shared text-panel HUD rendering in:
  - `simulators/common/scene_render_utils.h`
  - `simulators/common/scene_render_utils.cpp`
- Added shared titled info panels, legend panels, and centered banner helpers in:
  - `simulators/common/scene_render_utils.h`
  - `simulators/common/scene_render_utils.cpp`
- Added shared app-facing input translation in:
  - `app/scene_input_router.h`
  - `app/scene_input_router.cpp`
- Added shared control-binding formatting in:
  - `app/control_binding_formatter.h`
  - `app/control_binding_formatter.cpp`
- Added shared shell command/state catalogs in:
  - `app/shell_command_catalog.h`
  - `app/shell_command_catalog.cpp`
- Added shared shell text formatting in:
  - `app/shell_text_formatter.h`
  - `app/shell_text_formatter.cpp`
- Added shared scene status formatting in:
  - `app/scene_status_formatter.h`
  - `app/scene_status_formatter.cpp`
- Removed duplicated simulator input mapping logic from:
  - `app/runtime_controller.cpp`
  - `app/window_shell_runner.cpp`
- Removed duplicated control-binding string formatting from:
  - `app/shell_presenter.cpp`
  - `app/window_shell_runner.cpp`
- Removed duplicated shell command lists from:
  - `app/shell_presenter.cpp`
  - `app/window_shell_runner.cpp`
- Removed duplicated in-simulator summary formatting from:
  - `app/shell_presenter.cpp`
- Removed duplicated simulator HUD/legend/banner rendering from:
  - `simulators/lander/lander_scene.cpp`
  - `simulators/howitzer/howitzer_scene.cpp`
  - `simulators/chess/chess_scene.cpp`
  - `simulators/orbital/orbital_scene.cpp`

## Deferred On Purpose

- `Position` is still split by simulator family.
  - Apollo/Howitzer/window shell use the lander-era screen/world `Position`.
  - Chess uses a board-coordinate `Position`.
  - Orbital uses a meters/pixels orbital `Position`.
- These cannot be merged safely yet without adapters because they represent different domains.

## Next Cleanup Targets

1. Introduce explicit domain names for bridged legacy types.
   - `WorldPosition2D`
   - `BoardPosition`
   - `OrbitalWorldPosition`

2. Extend shared scene metadata/helpers only where duplication remains.
   - menu overlay notice rendering if window shell text grows further
   - shared simulator card/promo layout once prerecorded preview plumbing lands

3. Consolidate app-facing input mapping.
   - extend shared translator to menu navigation if that becomes duplicated

4. Audit physics/math overlaps between Apollo, Howitzer, and Orbital.
   - angle
   - acceleration
   - velocity
   - collision helpers
   - only unify after adapter coverage is stronger

## Guardrails

- Do not rewrite legacy simulator internals during cleanup unless behavior is already protected by tests.
- Prefer adapters and shared shell utilities before touching historical course-project code.
- Keep Chess and Orbital legacy bridges namespaced/renamed until type ownership is fully redesigned.
