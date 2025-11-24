#pragma once
#include <thread>
#include <atomic>
#include <string>

class TelemetryManager {
public:
    static TelemetryManager& instance() {
        static TelemetryManager inst;
        return inst;
    }

    void start();
    void stop();
    bool isRunning() const { return running; }

private:
    TelemetryManager() = default;
    ~TelemetryManager() = default;

    TelemetryManager(const TelemetryManager&) = delete;
    TelemetryManager& operator=(const TelemetryManager&) = delete;

    void telemetry_loop();

    std::thread worker;
    std::atomic<bool> running = false;
};
