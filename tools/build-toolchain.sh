#!/usr/bin/env bash
#
# [ M E G A D E V ]   a Sega Mega CD devkit
#
# Build the toolchain Megadev is tested with: binutils and GCC for bare-metal
# m68k-elf, defaulting to the 68000.
#
# usage: build-toolchain.sh [PREFIX]
#
#   PREFIX  install location, default /opt/megadev-toolchain
#   WORK    environment: build directory, kept afterward, with downloads in
#           WORK/dist. Default a temporary directory, removed afterward. Needs
#           about 4 gigabytes.
#   JOBS    environment: parallel jobs, default the number of CPUs
#
# Needs a C++ compiler, make, curl, git, xz, and the GMP, MPFR and MPC
# development headers.
#
# libgcc is not built: Megadev does not link it (see docs/dev_in_c.md).

set -euo pipefail

BINUTILS=binutils-2.46.1
BINUTILS_SHA512=a5c65e56e400ed3fb8906a995dbb93eb5bea54b16344244653d7f44ef29ceb60270da263b19d25302c37759784e14fcb4b9421b29e0e2c7f450bd99f6bb4595c
GCC=gcc-16.2.0
GCC_SHA512=c51c30ca7422d0cbecf504b2e0f33c3aca31e0f90a76b65217f465163fa6fa17b3f5de39e145c47e5bab90ac0ce7fff3b03c8d553ae36e01faaea5a50f8648d1
GNU_MIRROR=https://ftp.gnu.org/gnu
TARGET=m68k-elf

# Bump when anything above or in tools/patches changes. Shown by gcc --version.
TOOLCHAIN_REVISION=1

PREFIX=${1:-/opt/megadev-toolchain}
if [ -z "${WORK:-}" ]; then
  WORK=$(mktemp -d)
  trap 'rm -rf "$WORK"' EXIT
fi
JOBS=${JOBS:-$(nproc)}
PATCHES=$(cd "$(dirname "$0")/patches" && pwd)

fetch() {
  local url=$1 sha512=$2 file
  file=$WORK/dist/$(basename "$url")
  [ -f "$file" ] || curl -fL --retry 3 -o "$file" "$url"
  echo "$sha512  $file" | sha512sum -c -
}

mkdir -p "$WORK/dist" "$PREFIX"
PREFIX=$(cd "$PREFIX" && pwd)
export PATH=$PREFIX/bin:$PATH

fetch "$GNU_MIRROR/binutils/$BINUTILS.tar.xz" "$BINUTILS_SHA512"
fetch "$GNU_MIRROR/gcc/$GCC/$GCC.tar.xz" "$GCC_SHA512"

rm -rf "${WORK:?}/$BINUTILS" "${WORK:?}/$GCC" "$WORK/build-binutils" "$WORK/build-gcc"
tar -xf "$WORK/dist/$BINUTILS.tar.xz" -C "$WORK"
tar -xf "$WORK/dist/$GCC.tar.xz" -C "$WORK"

# git apply skips paths silently when run under another repository, so the
# source tree is made its own.
git -C "$WORK/$GCC" init -q
for p in "$PATCHES/$GCC"-*.patch; do
  git -C "$WORK/$GCC" apply --verbose "$p"
done

mkdir "$WORK/build-binutils"
cd "$WORK/build-binutils"
"../$BINUTILS/configure" \
  --target="$TARGET" \
  --prefix="$PREFIX" \
  --disable-nls \
  --disable-werror \
  MAKEINFO=true
make -j"$JOBS"
make install

mkdir "$WORK/build-gcc"
cd "$WORK/build-gcc"
"../$GCC/configure" \
  --target="$TARGET" \
  --prefix="$PREFIX" \
  --with-cpu=m68000 \
  --with-pkgversion="Megadev toolchain $TOOLCHAIN_REVISION" \
  --enable-languages=c,c++ \
  --without-headers \
  --with-newlib \
  --disable-multilib \
  --disable-nls \
  --disable-shared \
  --disable-threads \
  --disable-lto \
  --disable-plugin \
  --disable-libcc1 \
  --disable-bootstrap \
  MAKEINFO=true
make -j"$JOBS" all-gcc
make install-gcc

"$PREFIX/bin/$TARGET-gcc" --version | head -1
