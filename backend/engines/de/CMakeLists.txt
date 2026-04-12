set(SOURCES
    de_engine.cpp
    bindings.cpp
)

# Creater compiled module .so / .pyd for Python
pybind11_add_module(de_module ${SOURCES})

# Only needs links evo_core (that will bring core) and benchmarks
target_link_libraries(de_module
    PRIVATE
        evo_core
        benchmarks
)

# Sents compiled binary to frontend 
set_target_properties(de_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../frontend/native
)
