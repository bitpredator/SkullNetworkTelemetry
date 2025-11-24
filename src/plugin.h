#pragma once

// include degli header SCS (se li hai)
#include "scssdk.h"
#include "scssdk_plugin.h"
#include "scssdk_telemetry.h"

// Se stai compilando come C++ mantieni extern "C" per le API esportate
#ifdef __cplusplus
extern "C" {
#endif

// Funzioni richieste dall'SDK Telemetry (esportate)
SCSAPI_RESULT scs_telemetry_init(const scs_telemetry_init_params_t *params);
SCSAPI_VOID   scs_telemetry_shutdown();

#ifdef __cplusplus
} // extern "C"
#endif

// Funzione interna di pulizia richiamata da DllMain.
// Non deve essere esportata necessariamente; dichiariamola in C++
// (va bene anche in extern "C" ma non è obbligatorio).
void plugin_shutdown();
