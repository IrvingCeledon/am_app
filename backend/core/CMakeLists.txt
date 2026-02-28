add_library(core STATIC
    random.cpp
    logger.cpp
    # lo que agregues después
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(core PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -O2>
)
