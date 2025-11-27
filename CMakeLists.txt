cmake_minimum_required(VERSION 3.15)
project(SkullNetworkTelemetry LANGUAGES CXX)

add_definitions(-DWIN32_LEAN_AND_MEAN)

if(NOT WIN32)
    message(FATAL_ERROR "This project can be built only on Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# =========
#  SCS SDK INCLUDE (aggiornato)
# =========
set(SCS_SDK_INCLUDE ${CMAKE_SOURCE_DIR}/scs_sdk)

# =========
#  MAIN SOURCES
# =========
set(SOURCES
    src/dllmain.cpp
    src/plugin.cpp
    src/network_client.cpp
    src/telemetry_manager.cpp
    src/memory_scanner.cpp
    src/telemetry_payload.h

    src/offsets/offsets.cpp
    src/offsets/offsets_resolver.cpp
    src/offsets/pattern_finder.cpp
    src/offsets/truck_reader.cpp
)

# ========
#  INCLUDE DIRS
# ========
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/offsets
    ${CMAKE_SOURCE_DIR}/src/signatures
    ${SCS_SDK_INCLUDE}          # <--- FIX
)

# ========
#  DLL
# ========
add_library(SkullNetworkTelemetry SHARED
    ${SOURCES}
)

target_link_libraries(SkullNetworkTelemetry
    ws2_32
    psapi
)

target_compile_definitions(SkullNetworkTelemetry
    PRIVATE _CRT_SECURE_NO_WARNINGS
)
