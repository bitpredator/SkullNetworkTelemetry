#include "logger.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <mutex>

static std::mutex g_logMutex;
static std::ofstream g_logFile;

void Logger::initialize(const std::string& filename)
{
    std::lock_guard<std::mutex> lock(g_logMutex);

    if (g_logFile.is_open())
        return;

    g_logFile.open(filename, std::ios::out | std::ios::app);

    if (!g_logFile.is_open())
        MessageBoxA(NULL, "Impossibile aprire il file di log!", "Logger", MB_ICONERROR);
}

void Logger::shutdown()
{
    std::lock_guard<std::mutex> lock(g_logMutex);

    if (g_logFile.is_open())
        g_logFile.close();
}

void Logger::log(const std::string& level, const std::string& msg)
{
    std::lock_guard<std::mutex> lock(g_logMutex);

    if (g_logFile.is_open())
    {
        g_logFile << "[" << level << "] " << msg << std::endl;
        g_logFile.flush();
    }

    // stampa anche su console debug
    std::string debugMsg = "[" + level + "] " + msg + "\n";
    OutputDebugStringA(debugMsg.c_str());
}

void Logger::info(const std::string& msg)    { log("INFO", msg); }
void Logger::warn(const std::string& msg)    { log("WARN", msg); }
void Logger::error(const std::string& msg)   { log("ERROR", msg); }
