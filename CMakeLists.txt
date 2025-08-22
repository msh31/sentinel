cmake_minimum_required(VERSION 3.20)
project(sentinel VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build opptions
option(BUILD_EXAMPLES "Build example programs" ON)

# build the framework
add_library(sentinel SHARED
        src/core/logger.cpp
)

# headers
target_include_directories(sentinel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(BUILD_EXAMPLES)
    message(STATUS "Building examples")
    add_executable(logger_example examples/logger-example.cpp)
    target_link_libraries(logger_example sentinel)
endif()