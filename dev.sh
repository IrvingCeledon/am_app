#!/usr/bin/env bash
set -euo pipefail

#######################################
# Configuración
#######################################
PYTHON=python3
BUILD_DIR="backend/algorithms/ga/build"
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
        echo "Creando entorno virtual..."
        "$PYTHON" -m venv "$VENV_DIR"
    fi
}

#######################################
install_deps() {
    ensure_venv

    if [[ ! -f "$DEPS_FLAG" ]]; then
        echo "Instalando dependencias..."
        pip install --upgrade pip
        pip install PySide6 pybind11 mplcursors

        touch "$DEPS_FLAG"
    else
        echo "Dependencias ya instaladas"
    fi
}

#######################################
build_backend() {
    echo "Compilando backend v1..."
    cmake -S backend/algorithms/ga -B backend/algorithms/ga/build
    cmake --build backend/algorithms/ga/build -j$(nproc)
}

#######################################
run_app() {
    ensure_venv
    echo "Ejecutando frontend..."
    py -m frontend.main
}

#######################################
clean_build() {
    echo "Limpiando build..."
    rm -rf "$BUILD_DIR"
}

#######################################
clean_cache() {
    echo "Limpiando cache de Python..."
    find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
    find . -type f -name "*.pyc" -delete
}

#######################################
purge() {
    echo "Eliminando build, cache, data y venv..."
    rm -rf "$BUILD_DIR" "$VENV_DIR"
    rm -f "$DATA_DIR"/*.txt
}

#######################################
usage() {
    cat <<EOF
Uso: ./dev_v1.sh [command]

Commands:
  build        -> compila backend
  run          -> ejecuta la app
  deps         -> instala dependencias
  all          -> deps + build + run
  clean        -> elimina build + cache
  purge        -> elimina TODO (incluyendo venv)
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

