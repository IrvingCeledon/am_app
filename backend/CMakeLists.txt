cmake_minimum_required(VERSION 3.20)
project(Metaheuristics LANGUAGES CXX)

# ========= Global =========
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Python 3 REQUIRED COMPONENTS Interpreter Development)
set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 REQUIRED)

add_subdirectory(core)
add_subdirectory(evo_core)
add_subdirectory(algorithms)
