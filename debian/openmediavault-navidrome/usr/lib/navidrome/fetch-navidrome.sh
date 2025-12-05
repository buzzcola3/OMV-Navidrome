#!/bin/sh
set -e

DEFAULT_VERSION="0.58.5"
INSTALL_DIR="/usr/lib/navidrome"
TMPDIR=$(mktemp -d)

get_latest_version() {
    api_url="https://api.github.com/repos/navidrome/navidrome/releases/latest"
    latest=$(curl -fsSL "$api_url" 2>/dev/null | grep -m1 '"tag_name"' | sed -E 's/.*"v([^"[:space:]]+)".*/\1/' || true)
    if [ -z "$latest" ]; then
        echo "$DEFAULT_VERSION"
    else
        echo "$latest"
    fi
}

VERSION="${NAVIDROME_VERSION:-latest}"
if [ "$VERSION" = "latest" ]; then
    VERSION=$(get_latest_version)
fi

cleanup() {
    rm -rf "$TMPDIR"
}
trap cleanup EXIT

ARCH_OVERRIDE="${NAVIDROME_ARCH:-}";
if [ -n "$ARCH_OVERRIDE" ]; then
    ARCH="$ARCH_OVERRIDE"
else
    ARCH=$(dpkg --print-architecture)
fi
case "$ARCH" in
    amd64)
        ND_ARCH="linux_amd64"
        ;;
    arm64)
        ND_ARCH="linux_arm64"
        ;;
    armhf)
        ND_ARCH="linux_armv7"
        ;;
    *)
        echo "Unsupported architecture: $ARCH" >&2
        exit 1
        ;;
esac

TARBALL="navidrome_${VERSION}_${ND_ARCH}.tar.gz"
URL="https://github.com/navidrome/navidrome/releases/download/v${VERSION}/${TARBALL}"

if ! command -v curl >/dev/null 2>&1; then
    echo "curl is required to download Navidrome" >&2
    exit 1
fi

curl -fsSL "$URL" -o "$TMPDIR/$TARBALL"
tar -xzf "$TMPDIR/$TARBALL" -C "$TMPDIR"
install -d -m 0755 "$INSTALL_DIR"
install -m 0755 "$TMPDIR/navidrome" "$INSTALL_DIR/navidrome"
chown root:root "$INSTALL_DIR/navidrome"
echo "$VERSION" >"$INSTALL_DIR/.version"
chmod 0644 "$INSTALL_DIR/.version"
