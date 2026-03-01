add_library(benchmarks STATIC functions.cpp)
target_include_directories(benchmarks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Needs defined types in evo_core (like Genome or Fitness)
target_link_libraries(benchmarks PRIVATE evo_core)

pybind11_add_module(benchmarks_module bindings.cpp)
target_link_libraries(benchmarks_module PRIVATE benchmarks evo_core)
set_target_properties(benchmarks_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../frontend/native
)
