#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${1:-$ROOT_DIR/build}"
SOURCE_BIN="${2:-$BUILD_DIR/cpp_simulators_window}"
ICON_SOURCE="${3:-$ROOT_DIR/assets/icon/app-icon-source.png}"
APP_NAME="C++ Simulators"
APP_DIR="$ROOT_DIR/dist/$APP_NAME.app"
CONTENTS_DIR="$APP_DIR/Contents"
MACOS_DIR="$CONTENTS_DIR/MacOS"
RESOURCES_DIR="$CONTENTS_DIR/Resources"
ASSETS_DIR="$RESOURCES_DIR/assets"
PLIST_PATH="$CONTENTS_DIR/Info.plist"
ICON_COPY_PATH="$RESOURCES_DIR/app-icon-source.png"

if [[ ! -f "$SOURCE_BIN" ]]; then
  echo "Missing window binary: $SOURCE_BIN" >&2
  echo "Build cpp_simulators_window first." >&2
  exit 1
fi

rm -rf "$APP_DIR"
mkdir -p "$MACOS_DIR" "$ASSETS_DIR"

cp "$SOURCE_BIN" "$MACOS_DIR/$APP_NAME"
cp -R "$ROOT_DIR/assets/." "$ASSETS_DIR/"
cp "$ICON_SOURCE" "$ICON_COPY_PATH"

cat > "$PLIST_PATH" <<'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>CFBundleDevelopmentRegion</key>
  <string>en</string>
  <key>CFBundleExecutable</key>
  <string>C++ Simulators</string>
  <key>CFBundleIdentifier</key>
  <string>com.markvanhorn.cppsimulators</string>
  <key>CFBundleInfoDictionaryVersion</key>
  <string>6.0</string>
  <key>CFBundleName</key>
  <string>C++ Simulators</string>
  <key>CFBundlePackageType</key>
  <string>APPL</string>
  <key>CFBundleShortVersionString</key>
  <string>0.1.0</string>
  <key>CFBundleVersion</key>
  <string>1</string>
  <key>LSMinimumSystemVersion</key>
  <string>12.0</string>
  <key>NSHighResolutionCapable</key>
  <true/>
</dict>
</plist>
EOF

"$ROOT_DIR/scripts/apply_macos_file_icon.sh" "$APP_DIR" "$ICON_SOURCE" >/dev/null

echo "Packaged app bundle:"
echo "$APP_DIR"
