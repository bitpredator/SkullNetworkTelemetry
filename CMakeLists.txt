cmake_minimum_required(VERSION 3.20)

project(SkullNetworkTelemetry
    VERSION 1.0
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force MSVC runtime to be static or dynamic (default: dynamic)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# Source files
file(GLOB_RECURSE SRC_FILES
    "src/*.cpp"
    "src/**/*.cpp"
)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/offsets
    ${CMAKE_SOURCE_DIR}/src/signatures
    ${CMAKE_SOURCE_DIR}/extern  # ← json.hpp
)

# Build DLL
add_library(SkullNetworkTelemetry SHARED ${SRC_FILES})

# Windows libs
target_link_libraries(SkullNetworkTelemetry PRIVATE
    Ws2_32
    Psapi
)

# Output folder
set_target_properties(SkullNetworkTelemetry PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug
)

message(STATUS "SkullNetworkTelemetry configured successfully")
