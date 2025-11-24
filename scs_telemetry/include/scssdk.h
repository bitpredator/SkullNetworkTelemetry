#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Basic SCS API export macro
#define SCSAPI extern "C" __declspec(dllexport)

// Primitive types
typedef uint32_t scs_u32_t;
typedef uint64_t scs_u64_t;
typedef int32_t  scs_s32_t;
typedef int64_t  scs_s64_t;

typedef float  scs_float_t;
typedef double scs_double_t;

typedef uint8_t scs_bool_t;

// API result codes
typedef scs_u32_t SCSAPI_RESULT;
#define SCS_RESULT_ok               0
#define SCS_RESULT_unsupported      1
#define SCS_RESULT_generic_error    2

// VOID type for exported functions
#define SCSAPI_VOID void

#ifdef __cplusplus
}
#endif
