#pragma once
#include "scssdk.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    scs_u32_t version;
    void *common_api;
} scs_plugin_init_params_t;

#define SCS_PLUGIN_VERSION 1

#ifdef __cplusplus
}
#endif
