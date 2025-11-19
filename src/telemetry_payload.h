#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

// Struttura minimale di esempio per il payload telemetry.
// Questo verrà esteso con i campi reali (truck, job, coords, ecc.)
struct TelemetryPayload {
    std::string session_id;
    double lat = 0.0;
    double lon = 0.0;
    float speed_kph = 0.0f;
    float heading = 0.0f;
    std::string truck_make;
    std::string truck_model;

    std::string to_json() const {
        // Costruiamo JSON manualmente (escape semplice per i campi noti)
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(6);
        ss << "{";
        ss << "\"timestamp\":\"";
        // timestamp ISO8601 UTC
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        char buf[64];
        gmtime_s(&t, &t); // ensure thread-safe conversion
        std::tm tm;
        gmtime_s(&tm, &t);
        std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
        ss << buf << "\",";
        ss << "\"sessionId\":\"" << session_id << "\",";
        ss << "\"position\":{";
        ss << "\"lat\":" << lat << ",";
        ss << "\"lon\":" << lon << ",";
        ss << "\"heading\":" << heading;
        ss << "},";
        ss << "\"speed_kph\":" << speed_kph << ",";
        ss << "\"truck\":{";
        ss << "\"make\":\"" << truck_make << "\",";
        ss << "\"model\":\"" << truck_model << "\"";
        ss << "}";
        ss << "}";
        return ss.str();
    }
};
