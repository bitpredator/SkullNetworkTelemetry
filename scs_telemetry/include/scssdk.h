#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t  scs_bool_t;
typedef uint8_t  scs_u8_t;
typedef uint16_t scs_u16_t;
typedef uint32_t scs_u32_t;
typedef uint64_t scs_u64_t;
typedef int8_t   scs_s8_t;
typedef int32_t  scs_s32_t;
typedef int64_t  scs_s64_t;

typedef float    scs_float_t;
typedef double   scs_double_t;

typedef uint32_t scs_string_t; // not used here - keep for compatibility

#define SCS_TRUE  1
#define SCS_FALSE 0

// export macro (for plugin functions)
#ifdef _WIN32
  #define SCSAPI extern "C" __declspec(dllexport)
#else
  #define SCSAPI
#endif

#ifdef __cplusplus
}
#endif
