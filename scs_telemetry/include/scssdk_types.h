#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int8_t     scs_s8_t;
typedef uint8_t    scs_u8_t;

typedef int16_t    scs_s16_t;
typedef uint16_t   scs_u16_t;

typedef int32_t    scs_s32_t;
typedef uint32_t   scs_u32_t;

typedef int64_t    scs_s64_t;
typedef uint64_t   scs_u64_t;

typedef float      scs_float_t;
typedef double     scs_double_t;

// Boolean (0=false, 1=true)
typedef uint32_t   scs_bool_t;

// Version matching helper
typedef struct {
    scs_u32_t major;
    scs_u32_t minor;
    scs_u32_t patch;
} scs_sdk_version_t;

#ifdef __cplusplus
}
#endif
