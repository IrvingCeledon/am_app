#!/usr/bin/env bash
set -euo pipefail

#######################################
# Config
#######################################
PYTHON=python3
BUILD_DIR=build
VENV_DIR=venv
DATA_DIR=data

VENV_PY="$VENV_DIR/bin/python"
VENV_PIP="$VENV_DIR/bin/pip"
DEPS_FLAG="$VENV_DIR/.deps_installed"

#######################################
# Helpers
#######################################
py()  { "$VENV_PY" "$@"; }
pip() { "$VENV_PIP" "$@"; }

#######################################
ensure_venv() {
    if [[ ! -d "$VENV_DIR" ]]; then
        echo "Creating virtual environment..."
        "$PYTHON" -m venv "$VENV_DIR"
    fi
}

#######################################
install_deps() {
    ensure_venv

    if [[ ! -f "$DEPS_FLAG" ]]; then
        echo "Installing dependencies..."

        pip install --upgrade pip
        pip install PySide6 mplcursors

        touch "$DEPS_FLAG"
    else
        echo "Dependencies already installed"
    fi
}

#######################################
build_backend() {
    echo "Compiling backend..."
    cmake -S backend -B "$BUILD_DIR"
    cmake --build "$BUILD_DIR" -j"$(nproc)"
}

#######################################
run_app() {
    ensure_venv
    echo "Running frontend..."
    py -m frontend.main
}

#######################################
clean_build() {
    echo "Cleaning build..."
    rm -rf "$BUILD_DIR"
}

#######################################
clean_cache() {
    echo "Cleaning Python cache..."
    find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
    find . -type f -name "*.pyc" -delete
}

#######################################
purge() {
    echo "Removing build, cache, data and venv..."
    rm -rf "$BUILD_DIR" "$DATA_DIR" "$VENV_DIR"
}

#######################################
usage() {
    cat <<EOF
Usage: ./dev.sh [command]

Commands:
  build        -> compile backend
  run          -> run app
  deps         -> install dependencies
  all          -> deps + build + run
  clean        -> remove build + cache
  purge        -> remove EVERYTHING (including venv)
EOF
}

#######################################
# Entry point
#######################################
case "${1:-all}" in
    build) build_backend ;;
    run)   run_app ;;
    deps)  install_deps ;;
    all)
        install_deps
        build_backend
        run_app
        ;;
    clean)
        clean_build
        clean_cache
        ;;
    purge) purge ;;
    *) usage ;;
esac
