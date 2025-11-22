cmake_minimum_required(VERSION 3.15)
project(SkullNetworkTelemetry LANGUAGES CXX)

add_definitions(-DWIN32_LEAN_AND_MEAN)

# Windows only
if(NOT WIN32)
    message(FATAL_ERROR "This project can be built only on Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output DLL
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# =========
#  SOURCES
# =========
set(SOURCES
    src/dllmain.cpp
    src/plugin.cpp
    src/network_client.cpp
    src/telemetry_manager.cpp
    src/memory_scanner.cpp

    # telemetry payload header-only
    src/telemetry_payload.h

    # OFFSETS
    src/offsets/offsets.cpp
    src/offsets/offsets_resolver.cpp
    src/offsets/pattern_finder.cpp
    src/offsets/truck_reader.cpp

    # SIGNATURES
    src/signatures/signatures.h
    src/signatures/signatures_truck.h
    src/signatures/signatures_job.h
)

# =========
#  INCLUDE DIRS
# =========
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/offsets
    ${CMAKE_SOURCE_DIR}/src/signatures
    ${CMAKE_SOURCE_DIR}/scs_telemetry
)

# =========
#  CREATE DLL
# =========
add_library(SkullNetworkTelemetry SHARED ${SOURCES})

# =========
#  WINDOWS LIBRARIES
# =========
target_link_libraries(SkullNetworkTelemetry
    ws2_32
    psapi
)

# =========
#   DEFINITIONS
# =========
target_compile_definitions(SkullNetworkTelemetry
    PRIVATE _CRT_SECURE_NO_WARNINGS
)

# =========
#  JSON library workaround (header included manually)
# =========
# If you want to use embedded json.hpp (recommended)
target_include_directories(SkullNetworkTelemetry PRIVATE ${CMAKE_SOURCE_DIR}/external)

