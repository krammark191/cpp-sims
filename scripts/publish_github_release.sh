#!/bin/zsh

set -euo pipefail

if [[ $# -lt 1 || $# -gt 2 ]]; then
  echo "usage: $0 <tag> [release-title]" >&2
  exit 1
fi

TAG="$1"
TITLE="${2:-$1}"
REPO_SLUG="${REPO_SLUG:-krammark191/cpp-sims}"
DIST_DIR="${DIST_DIR:-dist}"
MAC_ASSET="${MAC_ASSET:-$DIST_DIR/C++ Simulators.dmg}"
WIN_ASSET="${WIN_ASSET:-$DIST_DIR/C++ Simulators.exe}"

if [[ ! -f "$MAC_ASSET" ]]; then
  echo "missing required asset: $MAC_ASSET" >&2
  exit 1
fi

ASSETS=("$MAC_ASSET")
if [[ -f "$WIN_ASSET" ]]; then
  ASSETS+=("$WIN_ASSET")
fi

creds="$(printf 'protocol=https\nhost=github.com\n\n' | git credential fill 2>/dev/null || true)"
user="$(printf '%s\n' "$creds" | sed -n 's/^username=//p' | head -n 1)"
pass="$(printf '%s\n' "$creds" | sed -n 's/^password=//p' | head -n 1)"

if [[ -z "$user" || -z "$pass" ]]; then
  echo "unable to locate GitHub credentials through git credential fill" >&2
  exit 1
fi

if ! git rev-parse -q --verify "refs/tags/$TAG" >/dev/null; then
  git tag "$TAG"
fi

git push origin "refs/tags/$TAG"

GITHUB_USER="$user" \
GITHUB_PASS="$pass" \
REPO_SLUG="$REPO_SLUG" \
RELEASE_TAG="$TAG" \
RELEASE_TITLE="$TITLE" \
/usr/bin/python3 - "$MAC_ASSET" "${ASSETS[@]:1}" <<'PY'
import base64
import json
import mimetypes
import os
import sys
import urllib.parse
import urllib.request

user = os.environ["GITHUB_USER"]
password = os.environ["GITHUB_PASS"]
repo = os.environ["REPO_SLUG"]
tag = os.environ["RELEASE_TAG"]
title = os.environ["RELEASE_TITLE"]
assets = sys.argv[1:]

auth = base64.b64encode(f"{user}:{password}".encode("utf-8")).decode("ascii")
api_headers = {
    "Accept": "application/vnd.github+json",
    "X-GitHub-Api-Version": "2022-11-28",
    "Authorization": f"Basic {auth}",
}


def request_json(method: str, url: str, data=None, extra_headers=None):
    body = None if data is None else json.dumps(data).encode("utf-8")
    headers = dict(api_headers)
    if extra_headers:
        headers.update(extra_headers)
    req = urllib.request.Request(url, data=body, headers=headers, method=method)
    with urllib.request.urlopen(req) as resp:
        return json.loads(resp.read().decode("utf-8"))


def request_raw(method: str, url: str, payload: bytes, content_type: str):
    headers = dict(api_headers)
    headers["Content-Type"] = content_type
    req = urllib.request.Request(url, data=payload, headers=headers, method=method)
    with urllib.request.urlopen(req) as resp:
        return json.loads(resp.read().decode("utf-8"))


release = None
try:
    release = request_json("GET", f"https://api.github.com/repos/{repo}/releases/tags/{tag}")
except Exception:
    release = request_json(
        "POST",
        f"https://api.github.com/repos/{repo}/releases",
        {
            "tag_name": tag,
            "name": title,
            "draft": False,
            "prerelease": False,
            "generate_release_notes": True,
        },
    )

upload_url = release["upload_url"].split("{", 1)[0]
existing_assets = {
    asset["name"]: asset["id"]
    for asset in request_json("GET", f"https://api.github.com/repos/{repo}/releases/{release['id']}/assets")
}

for asset_path in assets:
    name = os.path.basename(asset_path)
    if name in existing_assets:
        request_json(
            "DELETE",
            f"https://api.github.com/repos/{repo}/releases/assets/{existing_assets[name]}",
        )

    with open(asset_path, "rb") as handle:
        payload = handle.read()

    content_type = mimetypes.guess_type(name)[0] or "application/octet-stream"
    target = upload_url + "?name=" + urllib.parse.quote(name)
    request_raw("POST", target, payload, content_type)
    print(f"uploaded|{name}")

print(f"release|{release['html_url']}")
PY
