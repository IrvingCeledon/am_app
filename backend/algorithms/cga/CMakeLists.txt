set(SOURCES
    cga.cpp
    bindings.cpp
)

pybind11_add_module(cga_module ${SOURCES})

# target_include_directories(cga_module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cga_module PRIVATE evo_core)

set_target_properties(cga_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../frontend/native
)
