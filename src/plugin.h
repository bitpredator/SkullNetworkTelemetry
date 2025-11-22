#pragma once

#include <windows.h>

// Evita il name-mangling per le funzioni esportate verso ETS2/ATS
#ifdef __cplusplus
extern "C" {
#endif

// Funzioni richieste dal motore SCS (ETS2/ATS)
__declspec(dllexport) void scs_plugin_init(const unsigned int version, void* api);
__declspec(dllexport) void scs_plugin_shutdown();

#ifdef __cplusplus
}
#endif

// Funzioni interne al plugin (non esportate)
void plugin_shutdown();
