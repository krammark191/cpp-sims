# GitHub Releases

This project distributes packaged binaries through GitHub Releases.

## Supported Assets

- macOS:
  - `dist/C++ Simulators.dmg`
- Windows:
  - `dist/C++ Simulators.exe` when a Windows build is available

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
- optionally includes the Windows exe if it exists
- creates the Git tag locally if it does not already exist
- pushes the tag to `origin`
- creates or updates the matching GitHub release
- uploads the packaged assets to that release

## Inputs

- required:
  - `dist/C++ Simulators.dmg`
- optional:
  - `dist/C++ Simulators.exe`

## Authentication

The script uses GitHub credentials already available to Git on the local machine through `git credential fill`.

## Current Recommendation

- keep release asset names stable:
  - `C++ Simulators.dmg`
  - `C++ Simulators.exe`
- use semantic version tags such as `v0.1.0`
- publish packaged binaries through Releases rather than committing them into the repository
