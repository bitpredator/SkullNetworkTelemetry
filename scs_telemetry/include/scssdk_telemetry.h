#pragma once
#include "scssdk.h"
#include "scssdk_value.h"

#ifdef __cplusplus
extern "C" {
#endif

// Events
typedef enum {
    SCS_TELEMETRY_EVENT_frame_start = 0,
    SCS_TELEMETRY_EVENT_frame_end   = 1
} scs_telemetry_event_t;

// Callback prototype: called when a channel/variable has new value.
// name: string channel (e.g. "truck.speed"), value: pointer to scs_value_t
typedef void (*scs_telemetry_callback_t)(const char* name, const scs_value_t* value, void* user);

// register function signature we expose in init params
typedef scs_bool_t (*scs_register_channel_fn)(
    const char* channel_name,
    scs_value_type_t expected_type,
    scs_telemetry_event_t event,
    scs_telemetry_callback_t cb,
    void* user
);

// initialization struct passed to plugin init
typedef struct scs_telemetry_init_params_t {
    scs_u32_t telemetry_version;

    // function pointers provided by game/bridge
    scs_register_channel_fn register_channel;

    // reserved for future
    void* reserved[4];
} scs_telemetry_init_params_t;

// Common channel names (string constants) - modern style
// (these are common names used by many community telemetries)
static inline const char* SCS_CHANNEL_TRUCK_SPEED = "truck.speed";
static inline const char* SCS_CHANNEL_TRUCK_RPM   = "truck.engine.rpm";
static inline const char* SCS_CHANNEL_TRUCK_GEAR  = "truck.transmission.gear";
static inline const char* SCS_CHANNEL_TRUCK_POS   = "truck.position"; // vector (x,y,z)
static inline const char* SCS_CHANNEL_TRUCK_HEADING = "truck.heading";

#ifdef __cplusplus
}
#endif
