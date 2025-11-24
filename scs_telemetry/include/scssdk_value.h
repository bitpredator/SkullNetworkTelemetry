#pragma once

#include "scssdk.h"

#ifdef __cplusplus
extern "C" {
#endif

// Supported value types coming from ETS2 telemetry SDK
typedef enum {
    SCS_VALUE_TYPE_invalid = 0,
    SCS_VALUE_TYPE_bool,
    SCS_VALUE_TYPE_s32,
    SCS_VALUE_TYPE_u32,
    SCS_VALUE_TYPE_u64,
    SCS_VALUE_TYPE_float,
    SCS_VALUE_TYPE_double,
    SCS_VALUE_TYPE_string
} scs_value_type_t;

typedef struct {
    scs_value_type_t type;

    union {
        scs_bool_t   value_bool;
        scs_s32_t    value_s32;
        scs_u32_t    value_u32;
        scs_u64_t    value_u64;
        scs_float_t  value_float;
        scs_double_t value_double;
        const char*  value_string;
    };
} scs_value_t;

#ifdef __cplusplus
}
#endif
