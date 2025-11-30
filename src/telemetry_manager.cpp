// telemetry_manager.cpp
#include "telemetry_manager.h"
#include "network_client.h"
#include "logger.h"

#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <cstring>

// Nota:
// Questo file è intenzionalmente **indipendente** da funzioni/strutture
// di registrazione dell'SDK. La logica è:
// - se usi callbacks SDK, il tuo telemetry_callbacks.cpp chiama i setter
//   di TelemetryManager (setSpeed, setPosX, ...).
// - TelemetryManager NON tenta più di registrare canali via params,
//   in modo da evitare dipendenze fragile tra versioni SDK diverse.

bool TelemetryManager::initialize(const scs_telemetry_init_params_t* params)
{
    // Conserviamo il puntatore params solo per eventuale debug; non
    // tocchiamo membri interni perché le strutture SDK variano tra versioni.
    if (!params) {
        Logger::info("[TM] initialize: params == nullptr (ok, proceeding)");
    } else {
        Logger::info("[TM] initialize: params provided (no automatic register)");
    }

    // Restituiamo true: il manager è pronto a ricevere dati via i setter
    return true;
}

void TelemetryManager::start()
{
    bool expected = false;
    if (!running.compare_exchange_strong(expected, true)) {
        // già in esecuzione
        return;
    }

    worker = std::thread(&TelemetryManager::telemetry_loop, this);
    Logger::info("[TM] Telemetry thread started.");
}

void TelemetryManager::stop()
{
    bool expected = true;
    if (!running.compare_exchange_strong(expected, false)) {
        // già fermo
        return;
    }

    if (worker.joinable()) worker.join();
    Logger::info("[TM] Telemetry thread stopped.");
}

void TelemetryManager::telemetry_loop()
{
    using namespace std::chrono_literals;

    while (running.load()) {
        double x = 0.0, y = 0.0, z = 0.0;
        float sp = 0.0f, r = 0.0f, hd = 0.0f;

        {
            std::lock_guard<std::mutex> lk(data_mutex);
            x = pos_x; y = pos_y; z = pos_z;
            sp = speed_kph; r = rpm; hd = heading;
        }

        // Costruiamo JSON compatto
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(3);
        ss << "{\"truck\":{";
        ss << "\"speed\":" << sp << ",";
        ss << "\"rpm\":" << r << ",";
        ss << "\"heading\":" << hd;
        ss << "},\"game\":{";
        ss << "\"coordinates\":{\"x\":" << x << ",\"y\":" << y << ",\"z\":" << z << "}";
        ss << "}}";

        std::string payload = ss.str();

        // Log (leggero) e invio al server
        Logger::info("[TM] payload: " + payload);
        NetworkClient::instance().send_data(payload);

        std::this_thread::sleep_for(100ms);
    }
}

// --- Simple thread-safe setters (chiamabili dal telemetry_callbacks.cpp) ---
void TelemetryManager::setSpeed(float s) {
    std::lock_guard<std::mutex> lk(data_mutex);
    speed_kph = s;
}
void TelemetryManager::setRPM(float r) {
    std::lock_guard<std::mutex> lk(data_mutex);
    rpm = r;
}
void TelemetryManager::setHeading(float h) {
    std::lock_guard<std::mutex> lk(data_mutex);
    heading = h;
}
void TelemetryManager::setPosX(double v) {
    std::lock_guard<std::mutex> lk(data_mutex);
    pos_x = v;
}
void TelemetryManager::setPosY(double v) {
    std::lock_guard<std::mutex> lk(data_mutex);
    pos_y = v;
}
void TelemetryManager::setPosZ(double v) {
    std::lock_guard<std::mutex> lk(data_mutex);
    pos_z = v;
}
