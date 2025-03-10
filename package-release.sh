#!/bin/bash

set -e

shopt -s extglob

if [ -z "$1" ] || [ -z "$2" ]; then
  echo "Usage: $0 releasename destdir"
  exit 1
fi

VERSION="$1"
NVCUDA_SRC_DIR=$(dirname "$(readlink -f "$0")")
NVCUDA_BUILD_DIR=$(realpath "$2")"/nvcuda-$VERSION"

if [ -e "$NVCUDA_BUILD_DIR" ]; then
  echo "Build directory $NVCUDA_BUILD_DIR already exists"
  exit 1
fi

# build nvcuda

function build_arch_win {
  cd "$NVCUDA_SRC_DIR"

  meson setup                                            \
        --cross-file "$NVCUDA_SRC_DIR/cross-mingw$1.txt"  \
        --buildtype release                              \
        --prefix "$NVCUDA_BUILD_DIR"                     \
        --libdir lib                                     \
	--strip                                          \
        "$NVCUDA_BUILD_DIR/build.$1"

  cd "$NVCUDA_BUILD_DIR/build.$1"
  ninja install

  rm -R "$NVCUDA_BUILD_DIR/build.$1"
}

function build_arch_nix {
  cd "$NVCUDA_SRC_DIR"

  meson setup                                            \
        --cross-file "$NVCUDA_SRC_DIR/cross-wine$1.txt"  \
        --buildtype release                              \
        --prefix "$NVCUDA_BUILD_DIR"                     \
        --libdir lib                                     \
        --strip                                          \
        "$NVCUDA_BUILD_DIR/build.$1"

  cd "$NVCUDA_BUILD_DIR/build.$1"
  ninja install

  rm -R "$NVCUDA_BUILD_DIR/build.$1"
}

build_arch_win 64
build_arch_nix 64
build_arch_win 32
build_arch_nix 32

# cleanup
cp "$NVCUDA_SRC_DIR"/nvcuda_setup.sh $NVCUDA_BUILD_DIR
cd $NVCUDA_BUILD_DIR
find . -name \*.a -type f -delete
echo "Done building!"
