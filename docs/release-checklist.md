# Release Checklist

Use this checklist for tagged GitHub Releases.

## Preflight

- Confirm the working tree is clean:
  - `git status --short`
- Run the unit tests:
  - `./build/sampler_app_tests`
- Confirm the Xcode target still builds on macOS if the desktop app changed:
  - `xcodebuild -project "C++ Simulators.xcodeproj" -scheme "C++ Simulators" -configuration Debug -derivedDataPath build/xcode-derived build`
- Review [CHANGELOG.md](../CHANGELOG.md) and update the next release section if needed.
- Review [README.md](../README.md) if downloads, visuals, or platform support changed.

## macOS Packaging

- Rebuild the macOS app bundle and dmg:
  - `scripts/package_macos_app.sh`
  - `scripts/package_macos_dmg.sh`
- Verify the packaged outputs exist:
  - `dist/C++ Simulators.app`
  - `dist/C++ Simulators.dmg`

## GitHub Release

- Publish the release tag and upload the macOS dmg:
  - `scripts/publish_github_release.sh <tag>`
- Verify the GitHub release page exists:
  - `https://github.com/krammark191/cpp-sims/releases/latest`

## Windows Release

- Wait for `.github/workflows/windows-release.yml` to complete for the new tag.
- Confirm the release now includes:
  - `C++ Simulators-windows.zip`
  - `C++ Simulators.exe`
- Prefer the zip as the user-facing Windows download because it includes the runtime DLLs and assets.

## Final Checks

- Verify the latest download links in [README.md](../README.md) resolve correctly.
- Confirm the GitHub repo description and topics still match the current platform support.
- If the release changes app-facing copy or packaging behavior, update:
  - [README.md](../README.md)
  - [docs/github-releases.md](github-releases.md)
  - [docs/macos-packaging.md](macos-packaging.md)
  - [app/shell_presenter.cpp](../app/shell_presenter.cpp)
