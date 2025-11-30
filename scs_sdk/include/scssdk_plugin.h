#pragma once

// Dipendenze: scssdk_types.h fornisce i tipi scs_s32_t / scs_u32_t etc.
// scssdk.h fornisce macro generiche (se ne hai una versione) — non
// ci affidiamo a SCSAPI da scssdk.h; qui usiamo macro locali per export/callconv.
#include "scssdk_types.h"
#include "scssdk.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------
// Export / Calling convention
// -----------------------------
// Non ridefiniamo SCSAPI (potrebbe essere definito in scssdk.h).
// Usiamo invece macro locali, sicure e non invasive.
#if defined(_WIN32) || defined(_WIN64)
    #ifndef SCSAPI_EXPORT
        #define SCSAPI_EXPORT __declspec(dllexport)
    #endif
    #ifndef SCSAPI_CALL
        #define SCSAPI_CALL __stdcall
    #endif
#else
    #ifndef SCSAPI_EXPORT
        #define SCSAPI_EXPORT
    #endif
    #ifndef SCSAPI_CALL
        #define SCSAPI_CALL
    #endif
#endif

// -----------------------------
// Result type
// -----------------------------
typedef scs_s32_t scs_result_t;

#define SCS_RESULT_ok             0
#define SCS_RESULT_unsupported    1
#define SCS_RESULT_not_found      2
#define SCS_RESULT_refused        3
#define SCS_RESULT_generic_error  4

// -----------------------------
// Telemetry plugin init params
// -----------------------------
// Minimal, compatibile con il pattern usato in molti SDK.
// Se la SDK ufficiale fornisce una struct differente, sostituiscila.
typedef struct {
    scs_u32_t version;

    // pointer verso area "common" / API (engine-specific)
    void *common_api;
    // pointer verso telemetry API (fornisce register functions, ecc.)
    void *telemetry_api;
} scs_telemetry_init_params_t;

// -----------------------------
// Function pointer typedefs (calling convention explicit)
// -----------------------------
// Note: sono typedef utili se vuoi usare le function pointer; non
// sono obbligatori per dichiarare le funzioni esportate.
typedef scs_result_t (SCSAPI_CALL *scs_telemetry_init_t)(
    const scs_telemetry_init_params_t *params
);

typedef void (SCSAPI_CALL *scs_telemetry_shutdown_t)(void);

// -----------------------------
// Exported functions (the plugin must implement these)
// -----------------------------
SCSAPI_EXPORT scs_result_t SCSAPI_CALL scs_telemetry_init(
    const scs_telemetry_init_params_t *params
);

SCSAPI_EXPORT void SCSAPI_CALL scs_telemetry_shutdown(void);

#ifdef __cplusplus
} // extern "C"
#endif
