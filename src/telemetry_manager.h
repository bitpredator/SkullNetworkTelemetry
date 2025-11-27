#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <string>

#include "../scs_telemetry/include/scssdk_telemetry.h"

// forward declaration della callback
static void channel_callback_generic(const char* name, const scs_value_t* value, void* user);

class TelemetryManager {
public:
    static TelemetryManager& instance() {
        static TelemetryManager inst;
        return inst;
    }

    bool initialize(const scs_telemetry_init_params_t* params);

    void start();
    void stop();
    bool isRunning() const { return running; }

    // 👉 PERMETTE alla callback di accedere ai membri privati
    friend void ::channel_callback_generic(const char* name, const scs_value_t* value, void* user);

private:
    TelemetryManager() = default;
    ~TelemetryManager() = default;
    TelemetryManager(const TelemetryManager&) = delete;
    TelemetryManager& operator=(const TelemetryManager&) = delete;

    void telemetry_loop();

    std::thread worker;
    std::atomic<bool> running{false};
    std::mutex data_mutex;

    // dati runtime
    double pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;
    float speed_kph = 0.0f;
    float rpm = 0.0f;
    int gear = 0;
};
