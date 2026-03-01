set(SOURCES
    ga.cpp
    bindings.cpp
)

pybind11_add_module(ga_module ${SOURCES})

# target_include_directories(ga_module PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ga_module PRIVATE evo_core)

set_target_properties(ga_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../frontend/native
)
