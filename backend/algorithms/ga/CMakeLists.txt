cmake_minimum_required(VERSION 3.20)
project(GAAlgorithm LANGUAGES CXX)

# =========================
# Estándar de C++
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) 

# =========================
# Buscar Python moderno (evita warnings CMP0148)
# =========================
find_package(Python 3 REQUIRED COMPONENTS Interpreter Development)
set(PYBIND11_FINDPYTHON ON)  # para pybind11 use la búsqueda moderna

# =========================
# Pybind11
# =========================
find_package(pybind11 REQUIRED)

# =========================
# Archivos fuente
# =========================
set(SOURCES
    ga.cpp
    bindings.cpp
)

# =========================
# Crear módulo Python
# =========================
pybind11_add_module(ga_module ${SOURCES})

# =========================
# Incluye los headers
# =========================
target_include_directories(ga_module
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}  # para ga.hpp, individual.hpp, etc.
)

# =========================
# Opciones de compilación (opcional)
# =========================
target_compile_options(ga_module PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -O2>
)
