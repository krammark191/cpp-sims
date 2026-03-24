# macOS Packaging

This project now includes a local packaging script for the first macOS deliverable.

## Inputs

- Built GUI binary: `build/cpp_simulators_window`
- Asset tree: `assets/`
- App icon source: `assets/icon/app-icon-source.png`

## App Bundle Command

```sh
scripts/package_macos_app.sh
```

Optional arguments:

```sh
scripts/package_macos_app.sh <build-dir> <window-binary> <icon-source>
```

## Output

- App bundle: `dist/C++ Simulators.app`

The script creates a standard macOS bundle layout:

- `Contents/MacOS/C++ Simulators`
- `Contents/Resources/assets/...`
- `Contents/Resources/app-icon-source.png`
- `Contents/Info.plist`

## DMG Command

```sh
scripts/package_macos_dmg.sh
```

Optional arguments:

```sh
scripts/package_macos_dmg.sh <build-dir> <window-binary> <icon-source>
```

## DMG Output

- Disk image: `dist/C++ Simulators.dmg`

The dmg staging layout includes:

- `C++ Simulators.app`
- `Applications` symlink

## Current Notes

- The bundle stages the current preview/storyboard assets with the app.
- The bundle and dmg both receive a Finder icon from the current custom icon source.
- The icon source image is staged into the bundle resources for future replacement.
- The bundle identifier is currently `com.markvanhorn.cppsimulators`.
