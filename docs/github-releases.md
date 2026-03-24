# GitHub Releases

This project distributes packaged binaries through GitHub Releases.

## Supported Assets

- macOS:
  - `dist/C++ Simulators.dmg`
- Windows:
  - `dist/C++ Simulators.exe`
  - `dist/C++ Simulators-windows.zip`

The README points to the `latest` release page and to direct asset download URLs for those filenames.

## Publish Command

```sh
scripts/publish_github_release.sh <tag>
```

Examples:

```sh
scripts/publish_github_release.sh v0.1.0
scripts/publish_github_release.sh v0.2.0 "C++ Simulators v0.2.0"
```

## What The Script Does

- checks for the required macOS dmg asset
- pushes the release tag to GitHub
- creates or updates the matching GitHub release
- uploads the packaged macOS dmg immediately
- optionally uploads local Windows assets if they already exist
- lets the Windows GitHub Actions workflow build and attach the Windows zip and exe on release tags
- creates the Git tag locally if it does not already exist

## Inputs

- required:
  - `dist/C++ Simulators.dmg`
- optional:
  - `dist/C++ Simulators.exe`
  - `dist/C++ Simulators-windows.zip`

## Authentication

The script uses GitHub credentials already available to Git on the local machine through `git credential fill`.

## Current Recommendation

- keep release asset names stable:
  - `C++ Simulators.dmg`
  - `C++ Simulators.exe`
  - `C++ Simulators-windows.zip`
- use semantic version tags such as `v0.1.0`
- publish packaged binaries through Releases rather than committing them into the repository
- prefer the Windows zip as the user-facing download because the executable needs companion assets and runtime DLLs
- treat the macOS dmg as the manually packaged asset and the Windows package as the CI-built asset
