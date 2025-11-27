#pragma once

// === SCS SDK modern headers ===
#include <scssdk.h>
#include <scssdk_value.h>
#include <scssdk_telemetry.h>
#include <scssdk_plugin.h>

#ifdef __cplusplus
extern "C" {
#endif

// ===============================
//  Plugin required exports
// ===============================

// Called when the game loads the telemetry plugin
SCSAPI_RESULT scs_telemetry_init(
    const scs_telemetry_init_params_t* params
);

// Called when the game unloads the plugin
// NOTE: SCSAPI_VOID NON ESISTE – usare semplicemente void
void scs_telemetry_shutdown();

#ifdef __cplusplus
}
#endif

// ===============================
//  Internal C++ helpers
// ===============================

// Called from DllMain or your internal cleanup logic
void plugin_shutdown();
