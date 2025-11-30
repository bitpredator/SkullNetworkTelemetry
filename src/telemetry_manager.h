#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <string>
#include <sstream>
#include <iomanip>

#include <scssdk_telemetry.h>
#include <scssdk_value.h>

class TelemetryManager {
public:
    static TelemetryManager& instance() {
        static TelemetryManager inst;
        return inst;
    }

    // Called from plugin init with SDK params
    bool initialize(const scs_telemetry_init_params_t* params);

    void start();
    void stop();
    bool isRunning() const { return running.load(); }

    // Simple setters used by telemetry callbacks
    void setSpeed(float s);
    void setRPM(float r);
    void setHeading(float h);
    void setPosX(double v); 
    void setPosY(double v); 
    void setPosZ(double v);

    // 🔥 ADESSO I DATI SONO PUBLIC — ACCESSIBILI DAI CALLBACK
    std::mutex data_mutex;

    double pos_x = 0.0, pos_y = 0.0, pos_z = 0.0;
    float speed_kph = 0.0f;
    float rpm = 0.0f;
    float heading = 0.0f;

private:
    TelemetryManager() = default;
    ~TelemetryManager() = default;
    TelemetryManager(const TelemetryManager&) = delete;
    TelemetryManager& operator=(const TelemetryManager&) = delete;

    void telemetry_loop();

    std::thread worker;
    std::atomic<bool> running{false};
};
