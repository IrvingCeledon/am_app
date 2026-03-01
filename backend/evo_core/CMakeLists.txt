# INTERFACE means "not compile anything, just pass this rules whoever uses me" 
add_library(evo_core INTERFACE)

# Pass every includes from evo_core
target_include_directories(evo_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Any motor that links evo_core will link core automatically 
target_link_libraries(evo_core INTERFACE core)

pybind11_add_module(core_module bindings.cpp)
target_link_libraries(core_module PRIVATE evo_core)
set_target_properties(core_module PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../frontend/native
)
