cmake_minimum_required(VERSION 3.20)
project(sentinel VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# build / module options
option(BUILD_EXAMPLES "Build examples" OFF)

option(BUILD_VM_DETECTION "Build VM detection module" ON)
option(BUILD_ANTI_DEBUG "Build anti-debug module" ON)
option(BUILD_PROCESS_ANALYSIS "Build process analysis module" ON)
option(BUILD_PERSISTENCE "Build persistence module" ON)
option(BUILD_NETWORK_BEACON "Build network beacon module" ON)

# build the framework
add_library(sentinel STATIC
        src/core/logger.cpp
        src/core/utils/registry_helper.cpp
        src/core/utils/system_helper.cpp
        src/core/utils/process_helper.cpp
        src/core/utils/service_helper.cpp
)

# headers
target_include_directories(sentinel PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# linking to windows libraries for cross compiliation reasons
target_link_libraries(sentinel PRIVATE advapi32 psapi kernel32 user32)

# build conditions
if(BUILD_EXAMPLES)
    message(STATUS "Building examples")

    set(EXAMPLES
            logger-example
            vm_detection_example
            persistence-example
    )

    foreach(EXAMPLE ${EXAMPLES})
        add_executable(${EXAMPLE} examples/${EXAMPLE}.cpp)
        target_link_libraries(${EXAMPLE} sentinel)
        message(STATUS "  Building example: ${EXAMPLE}")
    endforeach()
endif()

if(BUILD_VM_DETECTION)
    target_sources(sentinel PRIVATE src/modules/vm_detection.cpp)
    target_compile_definitions(sentinel PRIVATE SENTINEL_HAS_VM_DETECTION)
endif()

#if(BUILD_ANTI_DEBUG)
#    target_sources(sentinel PRIVATE src/modules/anti_debug.cpp)
#    target_compile_definitions(sentinel PRIVATE SENTINEL_HAS_ANTI_DEBUG)
#endif()
#
#if(BUILD_PROCESS_ANALYSIS)
#    target_sources(sentinel PRIVATE src/modules/process_analysis.cpp)
#    target_compile_definitions(sentinel PRIVATE SENTINEL_HAS_PROCESS_ANALYSIS)
#endif()
#
if(BUILD_PERSISTENCE)
    target_sources(sentinel PRIVATE src/modules/persistence.cpp)
    target_compile_definitions(sentinel PRIVATE SENTINEL_HAS_PERSISTENCE)
endif()
#
#if(BUILD_NETWORK_BEACON)
#    target_sources(sentinel PRIVATE src/modules/network_beacon.cpp)
#    target_compile_definitions(sentinel PRIVATE SENTINEL_HAS_NETWORK_BEACON)
#endif()
