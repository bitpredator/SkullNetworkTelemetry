#include "scssdk.h"
#include "scssdk_telemetry.h"
#include "scssdk_value.h"

#include "TelemetryManager.h"

#include <iostream>

static void on_frame_start(scs_timestamp_t ts)
{
    TelemetryManager::instance().onFrameStart(ts);
}

static void on_frame_end(scs_timestamp_t ts)
{
    TelemetryManager::instance().onFrameEnd(ts);
}

static void on_config(const char *name, const scs_value_t *value)
{
    // Config data like truck model, brand, job info at spawn
    TelemetryManager::instance().onConfig(name, value);
}

static void on_telemetry(const char *name, const scs_value_t *value)
{
    auto &tm = TelemetryManager::instance();

    if (!value) return;

    if (strcmp(name, "truck.speed") == 0 && value->type == SCS_VALUE_TYPE_float)
        tm.setSpeed(value->value_float);

    else if (strcmp(name, "truck.engine.rpm") == 0 && value->type == SCS_VALUE_TYPE_float)
        tm.setRPM(value->value_float);

    else if (strcmp(name, "truck.lights.beam_low") == 0 && value->type == SCS_VALUE_TYPE_bool)
        tm.setLowBeam(value->value_bool);

    else if (strcmp(name, "truck.lights.beam_high") == 0 && value->type == SCS_VALUE_TYPE_bool)
        tm.setHighBeam(value->value_bool);

    else if (strcmp(name, "truck.lights.blinker.left") == 0 && value->type == SCS_VALUE_TYPE_bool)
        tm.setLeftBlinker(value->value_bool);

    else if (strcmp(name, "truck.lights.blinker.right") == 0 && value->type == SCS_VALUE_TYPE_bool)
        tm.setRightBlinker(value->value_bool);

    else if (strcmp(name, "truck.lights.hazard") == 0 && value->type == SCS_VALUE_TYPE_bool)
        tm.setHazard(value->value_bool);

    else if (strcmp(name, "truck.engine.on") == 0 && value->type == SCS_VALUE_TYPE_bool)
        tm.setEngineOn(value->value_bool);

    // POSITION
    else if (strcmp(name, "truck.position.x") == 0 && value->type == SCS_VALUE_TYPE_double)
        tm.setPosX(value->value_double);

    else if (strcmp(name, "truck.position.y") == 0 && value->type == SCS_VALUE_TYPE_double)
        tm.setPosY(value->value_double);

    else if (strcmp(name, "truck.position.z") == 0 && value->type == SCS_VALUE_TYPE_double)
        tm.setPosZ(value->value_double);

    else if (strcmp(name, "truck.orientation.heading") == 0 && value->type == SCS_VALUE_TYPE_float)
        tm.setHeading(value->value_float);

    // JOB DATA
    else if (strcmp(name, "job.income") == 0 && value->type == SCS_VALUE_TYPE_u64)
        tm.setJobIncome(value->value_u64);

    else if (strcmp(name, "job.distance") == 0 && value->type == SCS_VALUE_TYPE_float)
        tm.setJobDistance(value->value_float);

    else if (strcmp(name, "trailer.damage") == 0 && value->type == SCS_VALUE_TYPE_float)
        tm.setTrailerDamage(value->value_float);
}
