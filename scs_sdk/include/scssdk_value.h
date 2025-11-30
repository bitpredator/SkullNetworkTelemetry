#pragma once
#include "scssdk_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Value types
typedef enum scs_value_type_t {
    SCS_VALUE_TYPE_invalid = 0,

    SCS_VALUE_TYPE_bool,
    SCS_VALUE_TYPE_s32,
    SCS_VALUE_TYPE_u32,
    SCS_VALUE_TYPE_s64,
    SCS_VALUE_TYPE_u64,
    SCS_VALUE_TYPE_float,
    SCS_VALUE_TYPE_double,

    SCS_VALUE_TYPE_string,

    // Vector of 3 doubles (position, etc.)
    SCS_VALUE_TYPE_fvector,

    // Orientation (heading, pitch, roll)
    SCS_VALUE_TYPE_euler
} scs_value_type_t;

// ---- Value structures ----

typedef struct scs_value_bool_t {
    scs_bool_t value;
} scs_value_bool_t;

typedef struct scs_value_s32_t {
    scs_s32_t value;
} scs_value_s32_t;

typedef struct scs_value_u32_t {
    scs_u32_t value;
} scs_value_u32_t;

typedef struct scs_value_s64_t {
    scs_s64_t value;
} scs_value_s64_t;

typedef struct scs_value_u64_t {
    scs_u64_t value;
} scs_value_u64_t;

typedef struct scs_value_float_t {
    scs_float_t value;
} scs_value_float_t;

typedef struct scs_value_double_t {
    scs_double_t value;
} scs_value_double_t;

typedef struct scs_value_string_t {
    const scs_char_t *value;
} scs_value_string_t;

// Double vector (x, y, z)
typedef struct scs_value_fvector_t {
    scs_double_t x;
    scs_double_t y;
    scs_double_t z;
} scs_value_fvector_t;

// Orientation
typedef struct scs_value_euler_t {
    scs_float_t heading;
    scs_float_t pitch;
    scs_float_t roll;
} scs_value_euler_t;

// Generic type wrapper
typedef struct scs_value_t {
    scs_value_type_t type;
    union {
        scs_value_bool_t    value_bool;
        scs_value_s32_t     value_s32;
        scs_value_u32_t     value_u32;
        scs_value_s64_t     value_s64;
        scs_value_u64_t     value_u64;
        scs_value_float_t   value_float;
        scs_value_double_t  value_double;
        scs_value_string_t  value_string;
        scs_value_fvector_t value_fvector;
        scs_value_euler_t   value_euler;
    };
} scs_value_t;

#ifdef __cplusplus
}
#endif
