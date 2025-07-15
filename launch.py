#!/usr/bin/env python3
import subprocess
import os
import shutil
import argparse
import signal
import sys

BUILD_DIR = "build"
APP_DIR = "app"
DIST_DIR = os.path.join(APP_DIR, "dist")
APP_BUILD_CMD = ["npm", "run", "build"]
APP_DEV_CMD = ["npm", "run", "dev"]
LAUNCHER_PATH = os.path.join(BUILD_DIR, "elysium_launcher")

def install_depend():
    print("Installig npm dependencies syka")
    subprocess.run(["npm", "install"], cwd=APP_DIR, check=True)

def clean_build():
    os.makedirs(BUILD_DIR, exist_ok=True)
    shutil.rmtree(BUILD_DIR)
    os.makedirs(BUILD_DIR)

def build_app():
    install_depend()
    subprocess.run(APP_BUILD_CMD, cwd=APP_DIR, check=True)
    shutil.copytree(DIST_DIR, os.path.join(BUILD_DIR, "dist"))

def configure_cmake(extra_flags=None):
    cmd = ["cmake", "-B", BUILD_DIR, "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"]
    if extra_flags:
        cmd.extend(extra_flags)
    subprocess.run(cmd, check=True)

def build_cmake():
    subprocess.run(["cmake", "--build", BUILD_DIR], check=True)

def run_launcher():
    subprocess.run([LAUNCHER_PATH])

def run_dev():
    os.makedirs(BUILD_DIR, exist_ok=True)
    shutil.rmtree(BUILD_DIR)
    os.makedirs(BUILD_DIR)

    install_depend()

    # Start npm run dev
    npm_proc = subprocess.Popen(APP_DEV_CMD, cwd=APP_DIR)

    def cleanup(*args):
        print(f"\nStopping npm run dev (PID {npm_proc.pid})...")
        npm_proc.terminate()
        try:
            npm_proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            npm_proc.kill()
        sys.exit(0)

    # Catch termination signals
    signal.signal(signal.SIGINT, cleanup)
    signal.signal(signal.SIGTERM, cleanup)

    configure_cmake(extra_flags=["-DUSE_BUILTIN_WEBSERVER=OFF"])
    build_cmake()
    run_launcher()

def main():
    parser = argparse.ArgumentParser(description="Manage build and run of elysium project.")
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("build", help="Clean, build frontend and backend")
    subparsers.add_parser("run", help="Run compiled launcher")
    subparsers.add_parser("dev", help="Start dev server and run launcher")

    args = parser.parse_args()

    if args.command == "build":
        clean_build()
        build_app()
        configure_cmake()
        build_cmake()
    elif args.command == "run":
        run_launcher()
    elif args.command == "dev":
        run_dev()

if __name__ == "__main__":
    main()
