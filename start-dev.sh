#!/usr/bin/env sh
set -e

# Перейти в папку со скриптом
cd "$(dirname "$0")"

mkdir -p build

cd app

# Запускаем npm в фоне, сохраняем PID
npm run dev &
npm_pid=$!

cd ..

# При выходе из скрипта убиваем npm
cleanup() {
  echo "Stopping npm run dev (PID $npm_pid)..."
  kill $npm_pid 2>/dev/null || true
}
trap cleanup EXIT INT TERM

cmake -B build -DUSE_BUILTIN_WEBSERVER=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build

./build/elysium_launcher

