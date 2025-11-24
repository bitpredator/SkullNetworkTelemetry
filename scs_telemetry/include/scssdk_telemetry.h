#pragma once
#include "scssdk.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    scs_u32_t version;

    void *common_api;     // unused here
    void *telemetry_api;  // unused here

} scs_telemetry_init_params_t;

#define SCS_TELEMETRY_PLUGIN_VERSION 1

#ifdef __cplusplus
}
#endif
