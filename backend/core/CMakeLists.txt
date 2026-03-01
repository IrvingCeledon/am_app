add_library(core STATIC
    logger.cpp
    random.cpp
)

# PUBLIC makes that anyone that links 'core' inherits this includes path 
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
