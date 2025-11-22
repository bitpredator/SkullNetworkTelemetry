#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "plugin.h"
#include "network_client.h"
#include "telemetry_manager.h"

#pragma comment(lib, "Ws2_32.lib")

// ======================================================
//             GLOBAL STATE
// ======================================================
static bool g_pluginInitialized = false;

// ======================================================
//             INTERNAL FUNCTIONS
// ======================================================
bool plugin_startup()
{
    // --- Inizializza WinSock ---
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        OutputDebugStringA("[SkullTelemetry] WSAStartup FAILED\n");
        return false;
    }

    OutputDebugStringA("[SkullTelemetry] WSAStartup OK\n");

    // --- Avvia rete ---
    if (!NetworkClient::instance().init()) {
        OutputDebugStringA("[SkullTelemetry] Network init FAILED\n");
        return false;
    }

    // --- Avvia Telemetry Manager ETS2 ---
    if (!TelemetryManager::instance().init()) {
        OutputDebugStringA("[SkullTelemetry] TelemetryManager init FAILED\n");
        return false;
    }

    OutputDebugStringA("[SkullTelemetry] Plugin startup complete\n");
    return true;
}

void plugin_shutdown()
{
    if (!g_pluginInitialized)
        return;

    OutputDebugStringA("[SkullTelemetry] Shutting down\n");

    TelemetryManager::instance().shutdown();
    NetworkClient::instance().shutdown();

    WSACleanup();

    g_pluginInitialized = false;
}


// ======================================================
//             ETS2/ATS EXPORTED FUNCTIONS
// ======================================================

extern "C"
{

__declspec(dllexport)
void scs_plugin_init(const unsigned int version, void* api)
{
    OutputDebugStringA("[SkullTelemetry] scs_plugin_init called\n");

    if (g_pluginInitialized)
        return;

    if (!plugin_startup()) {
        OutputDebugStringA("[SkullTelemetry] Initialization failed\n");
        return;
    }

    g_pluginInitialized = true;
}

__declspec(dllexport)
void scs_plugin_shutdown()
{
    OutputDebugStringA("[SkullTelemetry] scs_plugin_shutdown called\n");
    plugin_shutdown();
}

} // extern "C"


// ======================================================
//       DLL ENTRY POINT (OPTIONALE MA CONSIGLIATO)
// ======================================================

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            OutputDebugStringA("[SkullTelemetry] DLL loaded\n");
            break;

        case DLL_PROCESS_DETACH:
            OutputDebugStringA("[SkullTelemetry] DLL unloaded\n");
            plugin_shutdown();  // failsafe
            break;
    }
    return TRUE;
}
