#include <scssdk.h>
#include <scssdk_telemetry.h>
#include <scssdk_value.h>
#include "telemetry_manager.h"

// Minimal example: if SDK calls global callbacks, map to TelemetryManager setters.
// If you register via params->register_channel (preferred), this file can be empty.

static void on_telemetry_cb(const char* name, const scs_value_t* value, void* user)
{
    if (!name || !value) return;
    auto &tm = TelemetryManager::instance();

    if (value->type == SCS_VALUE_TYPE_float) {
        if (strcmp(name, "truck.speed") == 0) tm.setSpeed(value->value_float.value * 3.6f);
        else if (strcmp(name, "truck.engine.rpm") == 0) tm.setRPM(value->value_float.value);
    } else if (value->type == SCS_VALUE_TYPE_double) {
        if (strcmp(name, "truck.position.x") == 0) tm.setPosX(value->value_double.value);
        else if (strcmp(name, "truck.position.y") == 0) tm.setPosY(value->value_double.value);
        else if (strcmp(name, "truck.position.z") == 0) tm.setPosZ(value->value_double.value);
    }
}

// You can call SDK registration functions from scs_telemetry_init if desired.
