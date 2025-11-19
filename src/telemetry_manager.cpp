#include "telemetry_manager.h"
#include "offsets.h"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <windows.h>

TelemetryManager::TelemetryManager() {}

TelemetryManager::~TelemetryManager() {
    stop();
}

//
// INITIALIZATION
//
bool TelemetryManager::initialize(const std::string& host, int port) {
    std::cout << "[TelemetryManager] Initializing..." << std::endl;

    if (!scanner.initialize()) {
        std::cerr << "[TelemetryManager] ERROR: MemoryScanner failed to start!" << std::endl;
        return false;
    }

    net_client = std::make_unique<NetworkClient>(host, port);

    if (!net_client->connect_socket()) {
        std::cerr << "[TelemetryManager] ERROR: Cannot connect to telemetry server!" << std::endl;
        return false;
    }

    std::cout << "[TelemetryManager] Ready." << std::endl;
    return true;
}

//
// START & STOP
//
void TelemetryManager::start() {
    if (running.load()) return;

    running.store(true);
    worker = std::thread(&TelemetryManager::worker_loop, this);

    std::cout << "[TelemetryManager] Worker thread started." << std::endl;
}

void TelemetryManager::stop() {
    if (!running.load()) return;

    running.store(false);

    if (worker.joinable()) {
        worker.join();
    }

    std::cout << "[TelemetryManager] Worker thread stopped." << std::endl;
}

//
// WORKER LOOP
//
void TelemetryManager::worker_loop() {
    while (running.load()) {
        TelemetryPayload payload;

        // Un ID univoco per la sessione
        payload.session_id = "SN-" + std::to_string(GetCurrentProcessId());

        // Lettura memoria → payload
        update_position(payload);
        update_speed(payload);
        update_heading(payload);
        update_truck_info(payload);

        // Convertiamo in JSON
        std::string json = payload.to_json();

        // Invio
        net_client->send(json);

        // 100 ms = 10Hz
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

//
// --- MEMORY READING FUNCTIONS ---
//

void TelemetryManager::update_position(TelemetryPayload& payload) {
    if (offset_player_position == 0)
        return;

    float pos[3] = { 0 };

    if (!scanner.read(offset_player_position, &pos, sizeof(pos))) {
        std::cerr << "[TelemetryManager] WARNING: Failed reading position" << std::endl;
        return;
    }

    // ETS2 coordinates (x,y,z) → per ora lat=x lon=z
    payload.lat = pos[0];
    payload.lon = pos[2];
}

void TelemetryManager::update_speed(TelemetryPayload& payload) {
    if (offset_player_speed == 0)
        return;

    float speed_ms = 0.0f;

    if (!scanner.read(offset_player_speed, &speed_ms, sizeof(speed_ms))) {
        std::cerr << "[TelemetryManager] WARNING: Failed reading speed" << std::endl;
        return;
    }

    payload.speed_kph = speed_ms * 3.6f;  // conversione m/s → km/h
}

void TelemetryManager::update_heading(TelemetryPayload& payload) {
    if (offset_player_heading == 0)
        return;

    float heading_rad = 0.0f;

    if (!scanner.read(offset_player_heading, &heading_rad, sizeof(heading_rad))) {
        std::cerr << "[TelemetryManager] WARNING: Failed reading heading" << std::endl;
        return;
    }

    payload.heading = heading_rad;
}

void TelemetryManager::update_truck_info(TelemetryPayload& payload) {
    if (offset_truck_name == 0)
        return;

    char buffer[64] = { 0 };

    if (!scanner.read(offset_truck_name, buffer, sizeof(buffer))) {
        std::cerr << "[TelemetryManager] WARNING: Failed reading truck name" << std::endl;
        return;
    }

    payload.truck_make = buffer;
    payload.truck_model = buffer; // per ora sono uguali, verranno separati
}
