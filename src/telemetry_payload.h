#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>    // per time_t, tm, std::strftime

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
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(6);

        // Otteniamo timestamp ISO8601 UTC in modo thread-safe (Windows)
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
#if defined(_WIN32)
        // gmtime_s è la versione thread-safe su MSVC
        gmtime_s(&tm, &t);
#else
        // su POSIX si può usare gmtime_r
        gmtime_r(&t, &tm);
#endif
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);

        ss << "{";
        ss << "\"timestamp\":\"" << buf << "\",";
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
