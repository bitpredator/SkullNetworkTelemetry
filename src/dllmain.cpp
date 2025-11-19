#include "plugin.h"

// DllMain minimale e sicuro.
// Evitiamo chiamate pesanti o thread creation direttamente in DllMain.
// DisableThreadLibraryCalls migliora le performance e stabilità.
BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hModule);
            break;
        case DLL_PROCESS_DETACH:
            // Chiamiamo cleanup centralizzato
            plugin_shutdown();
            break;
    }
    return TRUE;
}
