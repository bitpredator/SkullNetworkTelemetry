#pragma once

// SCS SDK headers (in CMake abbiamo aggiunto scs_sdk/include)
#include <scssdk.h>
#include <scssdk_plugin.h>
#include <scssdk_telemetry.h>

#ifdef __cplusplus
extern "C" {
#endif

// Plugin entry points required by SCS Telemetry API
scs_result_t SCSAPI_CALL scs_telemetry_init(const scs_telemetry_init_params_t* params);
void SCSAPI_CALL scs_telemetry_shutdown(void);

#ifdef __cplusplus
}
#endif

// Internal cleanup helper called from DllMain
void plugin_shutdown();
