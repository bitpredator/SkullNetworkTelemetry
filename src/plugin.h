#pragma once

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// Funzioni che ETS2/ATS si aspetta (C-style exports)
__declspec(dllexport) void scs_plugin_init(const unsigned int version, void* api);
__declspec(dllexport) void scs_plugin_shutdown();

#ifdef __cplusplus
}
#endif

// Funzione interna per DllMain
void plugin_shutdown();
