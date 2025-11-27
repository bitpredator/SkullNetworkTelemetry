#pragma once

#include <string>

class Logger {
public:
    // Scrive un messaggio semplice (no formatting)
    static void write(const std::string& msg);

    // Scrive un messaggio con formatting printf-style
    static void info(const char* fmt, ...);

    // Percorso del file di log (modificabile se vuoi)
    static inline const char* log_path = "C:/Users/Administrator/Desktop/SkullNetworkTelemetry/plugin_log.txt";
};
