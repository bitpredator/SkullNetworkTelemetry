#pragma once
#include <string>

class Logger {
public:
    static void initialize(const std::string& filename = "skn_telemetry.log");
    static void shutdown();

    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);

private:
    static void log(const std::string& level, const std::string& msg);
};
