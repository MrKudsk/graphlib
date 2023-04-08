#!/bin/sh

set -xe

COMMON_CFLAGS="-Wall -Wextra -ggdb -I. -I./build/ -I./thirdparty/"

build_sdl_demo() {
    NAME=$1
    clang $COMMON_CFLAGS -O2 -o ./bin/demos/$NAME.sdl -DPLATFORM=SDL_PLATFORM ./demo/$NAME.c =lm =lSDL2
}

# Build test
build_tests() {
    mkdir -p ./build/
    clang $COMMON_CFLAGS -o ./build/test test.c -lm
}

build_tests
mkdir -p ./bin/demos
build_sdl_demo triangle
