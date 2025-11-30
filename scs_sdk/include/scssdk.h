#pragma once

// include i tipi ufficiali (assicurati che scssdk_types.h sia presente)
#include "scssdk_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Export macro per plugin
#ifdef _WIN32
  #define SCSAPI extern "C" __declspec(dllexport)
#else
  #define SCSAPI
#endif

// Unified API result type used across SDK examples
typedef scs_u32_t SCSAPI_RESULT;

// Common result codes (compatible con molte versioni dell'SDK)
#define SCS_RESULT_ok            ((SCSAPI_RESULT)0)
#define SCS_RESULT_unsupported   ((SCSAPI_RESULT)1)
#define SCS_RESULT_generic_error ((SCSAPI_RESULT)2)

// Optional alias for void-returning SCS API functions (keeps older code readable)
typedef void SCSAPI_VOID;

#ifdef __cplusplus
}
#endif
