#!/usr/bin/env bash
set -euo pipefail

#######################################
# Config
#######################################
PYTHON=python3
BUILD_DIR=build
VENV_DIR=venv
DATA_DIR=data
VALGRIND="valgrind"

VENV_PY="$VENV_DIR/bin/python"
VENV_PIP="$VENV_DIR/bin/pip"
DEPS_FLAG="$VENV_DIR/.deps_installed"
VG_FLAGS="--leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log --verbose"
ASAN_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g -O1"

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
build_asan() {
    echo "Compiling backend with AddressSanitizer..."

    cmake -S backend -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_CXX_FLAGS="$ASAN_FLAGS" \
        -DCMAKE_C_FLAGS="$ASAN_FLAGS" \
        -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address" \
        -DCMAKE_SHARED_LINKER_FLAGS="-fsanitize=address"

    cmake --build "$BUILD_DIR" -j"$(nproc)"
}

#######################################
run_app() {
    ensure_venv
    echo "Running frontend..."
    py -m frontend.main "$@"
}

#######################################
run_valgrind() {
    ensure_venv
    echo "Running with Valgrind (memory check)..."
    $VALGRIND $VG_FLAGS "$VENV_PY" -m frontend.main "$@"
}

run_asan() {
    ensure_venv

    echo "Running with AddressSanitizer..."

    # más info útil en consola
    export ASAN_OPTIONS=detect_leaks=1:abort_on_error=1:symbolize=1

    py -m frontend.main "$@"
}

#######################################
clean_build() {
    echo "Cleaning build and native modules..."
    rm -rf "$BUILD_DIR"
    find frontend/native -type f -name "*.so" -delete
}

#######################################
clean_cache() {
    echo "Cleaning Python cache..."
    find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
    find . -type f -name "*.pyc" -delete
}

#######################################
purge() {
    echo "Removing build, data and venv..."
    rm -rf "$BUILD_DIR" "$DATA_DIR" "$VENV_DIR"
}

#######################################
dump_code() {
    echo "Exporting codebase to $DATA_DIR/full_codebase.txt..."
    
    # Aseguramos que el directorio data exista antes de guardar el archivo
    mkdir -p "$DATA_DIR"
    
    # Tu comando original, adaptado para apuntar a $DATA_DIR
    find backend frontend models services \( -path "*/build/*" -o -path "*/.git/*" -o -path "*/venv/*" -o -path "*/.venv/*" -o -path "*/__pycache__/*" -o -path "*/.idea/*" -o -path "*/data/*" \) -prune -o -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.py" \) -print | xargs awk 'FNR==1{print "\n\n// " "="x50 "\n// FILE: " FILENAME "\n// " "="x50 "\n"}1' > "$DATA_DIR/full_codebase.txt"
    
    echo "Done! Check $DATA_DIR/full_codebase.txt"
}

#######################################
usage() {
    cat <<EOF
Usage: ./dev.sh [command]

Commands:
  build        -> compile backend
  run          -> run app
  run-debug    -> run app with debug
  memcheck     -> run app with valgrind memory leak detection
  asan         -> build + run with AddressSanitizer (fast memory checks)
  deps         -> install dependencies
  dump         -> export codebase to a single text file inside data/
  all          -> deps + build + run
  clean        -> remove build + cache
  purge        -> remove EVERYTHING (including venv)
EOF
}

trap 'echo "BOAT GOES BINTED!!!!!"' EXIT

#######################################
# Entry point
#######################################
case "${1:-all}" in
    build) build_backend ;;
    run)   run_app ;;
    run-debug)    run_app --debug ;;
    memcheck) run_valgrind ;;
    asan)
        install_deps
        build_asan
        run_asan
    ;;
    deps)  install_deps ;;
    dump)  dump_code ;;
    all)
        install_deps
        build_backend
        run_app
        ;;
    clean)
        clean_build
        clean_cache
        ;;
    purge) 
        clean_build
        clean_cache
        purge 
        ;;
    *) usage ;;
esac
