#!/bin/zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
TARGET_PATH="${1:?Target file or app bundle path is required}"
ICON_SOURCE="${2:-$ROOT_DIR/assets/icon/app-icon-source.png}"
PYTHON_BIN="${PYTHON_BIN:-/Library/Frameworks/Python.framework/Versions/3.13/bin/python3}"

if [[ ! -e "$TARGET_PATH" ]]; then
  echo "Missing target path: $TARGET_PATH" >&2
  exit 1
fi

if [[ ! -f "$ICON_SOURCE" ]]; then
  echo "Missing icon source image: $ICON_SOURCE" >&2
  exit 1
fi

TARGET_PATH="$TARGET_PATH" ICON_SOURCE="$ICON_SOURCE" "$PYTHON_BIN" - <<'PY'
import os
from AppKit import NSImage, NSWorkspace

target_path = os.environ["TARGET_PATH"]
icon_source = os.environ["ICON_SOURCE"]

icon = NSImage.alloc().initWithContentsOfFile_(icon_source)
if icon is None:
    raise SystemExit(f"Unable to load icon source: {icon_source}")

workspace = NSWorkspace.sharedWorkspace()
if not workspace.setIcon_forFile_options_(icon, target_path, 0):
    raise SystemExit(f"Unable to apply icon to: {target_path}")
PY

echo "Applied icon:"
echo "$TARGET_PATH"
