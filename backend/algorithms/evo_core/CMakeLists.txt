add_library(evo_core INTERFACE)

target_include_directories(evo_core
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(evo_core
    INTERFACE core
)
