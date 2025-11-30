cmake_minimum_required(VERSION 3.15)
project(SkullNetworkTelemetry LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "This project can only be built on Windows.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-DWIN32_LEAN_AND_MEAN)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Path SDK
set(SCS_SDK_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/scs_sdk/include")
if(NOT EXISTS "${SCS_SDK_INCLUDE_DIR}/scssdk.h")
    message(FATAL_ERROR "SCS SDK headers not found in ${SCS_SDK_INCLUDE_DIR}")
endif()

include_directories(${SCS_SDK_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/external/nlohmann)

# Lista file esistenti
set(SOURCES
    src/dllmain.cpp
    src/plugin.cpp
    src/network_client.cpp
    src/telemetry_manager.cpp
    src/telemetry_callbacks.cpp
)

# Verifica se i file esistono fisicamente
foreach(FILE ${SOURCES})
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/${FILE}")
        message(FATAL_ERROR "Missing source file: ${FILE}")
    endif()
endforeach()

# Creazione DLL
add_library(SkullNetworkTelemetry SHARED ${SOURCES})

target_link_libraries(SkullNetworkTelemetry
    ws2_32
    psapi
)

target_compile_definitions(SkullNetworkTelemetry PRIVATE _CRT_SECURE_NO_WARNINGS)

if(MSVC)
    target_compile_options(SkullNetworkTelemetry PRIVATE /W3 /EHsc)
endif()
