cmake_minimum_required(VERSION 3.20)

project(SkullNetworkTelemetry
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Require 64-bit
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit build is supported")
endif()

# Source directories
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Collect all .cpp files recursively (keeps CMake reconfigure when files change)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${SRC_DIR}/*.cpp"
)

# If no sources found, print helpful message and error
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in ${SRC_DIR}. Did you forget to add .cpp files? Checked pattern: ${SRC_DIR}/*.cpp")
endif()

# Include dirs
include_directories(
    ${SRC_DIR}
    ${SRC_DIR}/offsets
    ${SRC_DIR}/signatures
)

# Export all symbols automatically for DLL
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(SkullNetworkTelemetry SHARED ${SOURCES})

# Link required Windows libraries
if (WIN32)
    target_link_libraries(SkullNetworkTelemetry PRIVATE ws2_32)
endif()

# Output directories
set_target_properties(SkullNetworkTelemetry PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/build/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/build/Release"
)

# Compiler warnings
if(MSVC)
    target_compile_options(SkullNetworkTelemetry PRIVATE /W4 /permissive- /EHsc)
else()
    target_compile_options(SkullNetworkTelemetry PRIVATE -Wall -Wextra -pedantic)
endif()

message(STATUS "SkullNetworkTelemetry project configured successfully")
