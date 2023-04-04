#!/bin/sh

set -xe

COMMON_CFLAGS="-Wall -Wextra -ggdb -I. -I./build/ -I./thirdparty/"

mkdir -p ./build/

# Build test
clang $COMMON_CFLAGS -o ./build/test test.c -lm
