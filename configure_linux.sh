#!/bin/sh
mkdir -p build-linux
cd build-linux && \
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=../Library/cmake/aarch64-linux-gnu.cmake \
        -GNinja
