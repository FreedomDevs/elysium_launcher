#!/usr/bin/env sh
set -e

mkdir -p build

cd app
npm run build
cd ..
cp -rf app/dist/ build/

cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
