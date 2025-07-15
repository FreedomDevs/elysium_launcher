#!/usr/bin/env sh
set -e

mkdir -p build
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
