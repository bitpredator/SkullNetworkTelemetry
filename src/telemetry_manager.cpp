#include "telemetry_manager.h"
#include "network_client.h"
#include <chrono>

void TelemetryManager::start() {
    if (running) return;

    running = true;
    worker = std::thread(&TelemetryManager::telemetry_loop, this);
}

void TelemetryManager::stop() {
    if (!running) return;

    running = false;
    if (worker.joinable())
        worker.join();
}

void TelemetryManager::telemetry_loop() {
    while (running) {
        std::string payload = "{\"test\":true}"; // placeholder
        NetworkClient::instance().send_data(payload);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
