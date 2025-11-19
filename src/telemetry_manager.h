#pragma once
#include <atomic>
#include <thread>
#include <memory>

#include "memory_scanner.h"
#include "network_client.h"
#include "telemetry_payload.h"

class TelemetryManager {
public:
    TelemetryManager();
    ~TelemetryManager();

    bool initialize(const std::string& host, int port);
    void start();
    void stop();

private:
    void worker_loop();

    std::atomic<bool> running{ false };
    std::thread worker;

    MemoryScanner scanner;
    std::unique_ptr<NetworkClient> net_client;

    // Offsets (da popolare nella fase 2)
    uintptr_t offset_player_position = 0;
    uintptr_t offset_player_speed = 0;
    uintptr_t offset_player_heading = 0;
    uintptr_t offset_truck_name = 0;

    // Funzioni interne per aggiornare i valori
    void update_position(TelemetryPayload& payload);
    void update_speed(TelemetryPayload& payload);
    void update_heading(TelemetryPayload& payload);
    void update_truck_info(TelemetryPayload& payload);
};
