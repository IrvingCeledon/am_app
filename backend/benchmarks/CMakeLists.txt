add_library(benchmarks STATIC functions.cpp)
target_include_directories(benchmarks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(benchmarks PRIVATE evo_core)

pybind11_add_module(benchmarks_module bindings.cpp)
target_link_libraries(benchmarks_module PRIVATE benchmarks evo_core)
set_target_properties(benchmarks_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../frontend/native
)

# ==========================================
# Kinematic Module (IK Project)
# ==========================================
add_library(kinematic STATIC kinematic.cpp)
target_include_directories(kinematic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(kinematic PRIVATE evo_core)

pybind11_add_module(kinematic_module kinematic_bindings.cpp)
target_link_libraries(kinematic_module PRIVATE kinematic evo_core)

set_target_properties(kinematic_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../frontend/native
)
