cmake_minimum_required(VERSION 3.20)

# Project
project(SkullNetworkTelemetry
    VERSION 1.0
    LANGUAGES CXX
)

# Imposta C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compilazione a 64-bit
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only 64-bit build is supported")
endif()

# Definisce cartelle sorgenti
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(OFFSETS_DIR "${SRC_DIR}/offsets")
set(SIGNATURES_DIR "${SRC_DIR}/signatures")

# Lista completa dei file sorgenti
set(SOURCES
    ${SRC_DIR}/dllmain.cpp
    ${SRC_DIR}/plugin.cpp
    ${SRC_DIR}/telemetry_manager.cpp
    ${SRC_DIR}/network_client.cpp
    ${SRC_DIR}/telemetry_payload.cpp
    ${OFFSETS_DIR}/offsets_resolver.cpp
    ${OFFSETS_DIR}/memory_scanner.cpp
    ${OFFSETS_DIR}/pattern_finder.cpp
)

# Include directories
include_directories(
    ${SRC_DIR}
    ${OFFSETS_DIR}
    ${SIGNATURES_DIR}
)

# Setta output DLL
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(SkullNetworkTelemetry SHARED ${SOURCES})

# Link con librerie Windows necessarie
target_link_libraries(SkullNetworkTelemetry
    PRIVATE
    ws2_32    # per socket
)

# Impostazioni per la build in Debug e Release
set_target_properties(SkullNetworkTelemetry PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/build/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/build/Release"
)

# Opzioni compilatore Windows
if(MSVC)
    target_compile_options(SkullNetworkTelemetry PRIVATE /W4 /permissive- /EHsc)
else()
    target_compile_options(SkullNetworkTelemetry PRIVATE -Wall -Wextra -pedantic)
endif()

message(STATUS "SkullNetworkTelemetry project configured successfully")
