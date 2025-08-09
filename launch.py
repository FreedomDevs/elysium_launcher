#!/usr/bin/env python3
import subprocess
import os
import shutil
import argparse
import signal
import sys

BUILD_DIR = "build"
APP_DIR = "app"
EL_CORE_LIB_DIR = "libs/el-core-lib"
DIST_DIR = os.path.join(APP_DIR, "dist")
APP_BUILD_CMD = ["npm", "run", "build"]
APP_DEV_CMD = ["npm", "run", "dev"]
LAUNCHER_PATH = os.path.join(BUILD_DIR, "elysium_launcher")

def install_depend():
    print("Installig npm dependencies")
    subprocess.run(["npm", "install"], cwd=APP_DIR, check=True)
    print("Installig cargo dependencies")
    subprocess.run(["cargo", "install", "cbindgen"], check=True)

def clean_build():
    os.makedirs(BUILD_DIR, exist_ok=True)
    shutil.rmtree(BUILD_DIR)
    os.makedirs(BUILD_DIR)

def build_app():
    subprocess.run(APP_BUILD_CMD, cwd=APP_DIR, check=True)
    shutil.copytree(DIST_DIR, os.path.join(BUILD_DIR, "dist"), dirs_exist_ok=True)

def configure_cmake(extra_flags=None):
    cmd = ["cmake", "-B", BUILD_DIR, "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"]
    if extra_flags:
        cmd.extend(extra_flags)
    subprocess.run(cmd, check=True)

def build_cmake():
    subprocess.run(["cmake", "--build", BUILD_DIR, "--", "-j", "12"], check=True)

def run_launcher():
    return subprocess.Popen([LAUNCHER_PATH])

def run_launcher_debug():
    return subprocess.Popen(["gdb", "--batch", "-ex", "run", "-ex", "bt", "-ex", "quit", "--args", LAUNCHER_PATH])

def run_dev():
    # Start npm run dev
    npm_proc = subprocess.Popen(APP_DEV_CMD, cwd=APP_DIR)

    configure_cmake(extra_flags=["-DUSE_BUILTIN_WEBSERVER=OFF", "-DCMAKE_BUILD_TYPE=Debug"])
    build_cmake()
    launcher_proc = run_launcher_debug()

    def cleanup(npm_only: bool = False, *args):
        print(f"\nStopping npm run dev (PID {npm_proc.pid})...")
        npm_proc.terminate()

        if not npm_only:
            print(f"Stopping launcher (PID {launcher_proc.pid})...")
            launcher_proc.terminate()

            try:
                launcher_proc.wait(timeout=5)
            except subprocess.TimeoutExpired:
                print("launcher_proc не завершился, убиваем...")
                launcher_proc.kill()

        try:
            npm_proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            print("npm_proc не завершился, убиваем...")
            try:
                os.killpg(os.getpgid(npm_proc.pid), signal.SIGKILL)
            except ProcessLookupError:
                pass
        sys.exit(0)

    try:
        launcher_proc.wait()
        cleanup(npm_only=True)
    except KeyboardInterrupt:
        cleanup()

def run_build():
    build_app()
    configure_cmake()
    build_cmake()

def run_and_build_launcher():
    run_build()
    run_launcher()

def main():
    parser = argparse.ArgumentParser(description="Manage build and run of elysium project.")
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("build", help="Clean, build frontend and backend")
    subparsers.add_parser("run", help="Run compiled launcher")
    subparsers.add_parser("runbuild", help="Run compiled launcher")
    subparsers.add_parser("dev", help="Start dev server and run launcher")
    subparsers.add_parser("install", help="Install dependencies")
    subparsers.add_parser("clean", help="Clean build")

    args = parser.parse_args()

    if args.command == "build":
        run_build()
    elif args.command == "run":
        run_launcher()
    elif args.command == "runbuild":
        run_and_build_launcher()
    elif args.command == "dev":
        run_dev()
    elif args.command == "install":
        install_depend()
    elif args.command == "clean":
        clean_build()

if __name__ == "__main__":
    main()
