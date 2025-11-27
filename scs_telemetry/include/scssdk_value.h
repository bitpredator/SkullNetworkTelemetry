#pragma once
#include "scssdk.h"

#ifdef __cplusplus
extern "C" {
#endif

// Common vector type used by telemetry (x,y,z)
typedef struct scs_vector_t {
    scs_double_t x;
    scs_double_t y;
    scs_double_t z;
} scs_vector_t;

// Value types
typedef enum {
    SCS_VALUE_TYPE_unknown = 0,
    SCS_VALUE_TYPE_float = 1,
    SCS_VALUE_TYPE_double = 2,
    SCS_VALUE_TYPE_s32 = 3,
    SCS_VALUE_TYPE_s64 = 4,
    SCS_VALUE_TYPE_string = 5,
    SCS_VALUE_TYPE_vector = 6 // scs_vector_t (double)
} scs_value_type_t;

// Generic value union
typedef struct scs_value_t {
    scs_value_type_t type;
    union {
        scs_float_t  v_float;
        scs_double_t v_double;
        scs_s32_t    v_s32;
        scs_s64_t    v_s64;
        const char*  v_string;
        scs_vector_t v_vector;
    } u;
} scs_value_t;

#ifdef __cplusplus
}
#endif
