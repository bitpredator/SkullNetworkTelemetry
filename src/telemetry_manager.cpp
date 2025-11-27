#include "telemetry_manager.h"
#include "network_client.h"
#include "logger.h"

#include <chrono>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstring>

static void channel_callback_generic(const char* name, const scs_value_t* value, void* user)
{
    TelemetryManager* mgr = reinterpret_cast<TelemetryManager*>(user);
    if (!mgr || !value || !name) return;

    std::lock_guard<std::mutex> lk(mgr->data_mutex);

    if (strcmp(name, SCS_CHANNEL_TRUCK_SPEED) == 0 && value->type == SCS_VALUE_TYPE_float) {
        mgr->speed_kph = value->u.v_float * 3.6f; // if value is m/s convert
    } else if (strcmp(name, SCS_CHANNEL_TRUCK_RPM) == 0 && value->type == SCS_VALUE_TYPE_float) {
        mgr->rpm = value->u.v_float;
    } else if (strcmp(name, SCS_CHANNEL_TRUCK_GEAR) == 0 && value->type == SCS_VALUE_TYPE_s32) {
        mgr->gear = value->u.v_s32;
    } else if (strcmp(name, SCS_CHANNEL_TRUCK_POS) == 0 && value->type == SCS_VALUE_TYPE_vector) {
        mgr->pos_x = value->u.v_vector.x;
        mgr->pos_y = value->u.v_vector.y;
        mgr->pos_z = value->u.v_vector.z;
    } else if (strcmp(name, SCS_CHANNEL_TRUCK_HEADING) == 0) {
        // heading might be provided as float/double in radians
        if (value->type == SCS_VALUE_TYPE_float) mgr->pos_y = value->u.v_float; // example fallback
        else if (value->type == SCS_VALUE_TYPE_double) mgr->pos_y = (float)value->u.v_double;
    }
}

bool TelemetryManager::initialize(const scs_telemetry_init_params_t* params)
{
    if (!params || !params->register_channel) {
        Logger::info("[TM] initialize: missing params/register_channel");
        return false;
    }

    // Register the channels we care about
    Logger::info("[TM] Registering telemetry channels...");

    // Using our generic callback pointer with 'this' as user
    params->register_channel(SCS_CHANNEL_TRUCK_SPEED,  SCS_VALUE_TYPE_float,  SCS_TELEMETRY_EVENT_frame_end, channel_callback_generic, this);
    params->register_channel(SCS_CHANNEL_TRUCK_RPM,    SCS_VALUE_TYPE_float,  SCS_TELEMETRY_EVENT_frame_end, channel_callback_generic, this);
    params->register_channel(SCS_CHANNEL_TRUCK_GEAR,   SCS_VALUE_TYPE_s32,    SCS_TELEMETRY_EVENT_frame_end, channel_callback_generic, this);
    params->register_channel(SCS_CHANNEL_TRUCK_POS,    SCS_VALUE_TYPE_vector, SCS_TELEMETRY_EVENT_frame_end, channel_callback_generic, this);
    params->register_channel(SCS_CHANNEL_TRUCK_HEADING,SCS_VALUE_TYPE_double, SCS_TELEMETRY_EVENT_frame_end, channel_callback_generic, this);

    Logger::info("[TM] Channels registered.");

    return true;
}

void TelemetryManager::start()
{
    if (running.load()) return;
    running.store(true);
    worker = std::thread(&TelemetryManager::telemetry_loop, this);
    Logger::info("[TM] Telemetry thread started.");
}

void TelemetryManager::stop()
{
    if (!running.load()) return;
    running.store(false);
    if (worker.joinable()) worker.join();
    Logger::info("[TM] Telemetry thread stopped.");
}

void TelemetryManager::telemetry_loop()
{
    using namespace std::chrono_literals;
    while (running.load()) {
        // prepare JSON snapshot
        double x,y,z;
        float speed, rrpm;
        int g;

        {
            std::lock_guard<std::mutex> lk(data_mutex);
            x = pos_x; y = pos_y; z = pos_z;
            speed = speed_kph; rrpm = rpm; g = gear;
        }

        // build compact JSON
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(3);
        ss << "{\"truck\":{";
        ss << "\"speed\":" << speed << ",";
        ss << "\"rpm\":" << rrpm << ",";
        ss << "\"gear\":" << g;
        ss << "},\"game\":{";
        ss << "\"coordinates\":{\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << "}";
        ss << "}}";

        std::string payload = ss.str();

        Logger::info("[TM] Sending payload: %s", payload.c_str());
        NetworkClient::instance().send_data(payload);

        std::this_thread::sleep_for(100ms);
    }
}
