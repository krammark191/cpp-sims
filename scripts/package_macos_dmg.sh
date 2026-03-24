#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${1:-$ROOT_DIR/build}"
SOURCE_BIN="${2:-$BUILD_DIR/cpp_simulators_window}"
ICON_SOURCE="${3:-$ROOT_DIR/assets/icon/app-icon-source.png}"
APP_NAME="C++ Simulators"
APP_DIR="$ROOT_DIR/dist/$APP_NAME.app"
DMG_DIR="$ROOT_DIR/dist"
DMG_PATH="$DMG_DIR/$APP_NAME.dmg"
STAGING_DIR="$ROOT_DIR/build/dmg-staging"
RW_DMG_PATH="$ROOT_DIR/build/$APP_NAME-temp.dmg"
VOLUME_NAME="$APP_NAME Installer"

"$ROOT_DIR/scripts/package_macos_app.sh" "$BUILD_DIR" "$SOURCE_BIN" "$ICON_SOURCE" >/dev/null

rm -rf "$STAGING_DIR"
mkdir -p "$STAGING_DIR"
cp -R "$APP_DIR" "$STAGING_DIR/"
ln -s /Applications "$STAGING_DIR/Applications"

rm -f "$RW_DMG_PATH" "$DMG_PATH"

hdiutil create \
  -volname "$VOLUME_NAME" \
  -srcfolder "$STAGING_DIR" \
  -ov \
  -format UDRW \
  "$RW_DMG_PATH" >/dev/null

hdiutil convert "$RW_DMG_PATH" \
  -ov \
  -format UDZO \
  -imagekey zlib-level=9 \
  -o "$DMG_PATH" >/dev/null

rm -f "$RW_DMG_PATH"

"$ROOT_DIR/scripts/apply_macos_file_icon.sh" "$DMG_PATH" "$ICON_SOURCE" >/dev/null

echo "Packaged dmg:"
echo "$DMG_PATH"
