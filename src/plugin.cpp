#include "plugin.h"
#include "telemetry_manager.h"
#include "offsets_resolver.h"
#include <iostream>
#include <memory>

static std::unique_ptr<TelemetryManager> g_manager;

bool plugin_start()
{
    std::cout << "[SkullTelemetry] Starting plugin..." << std::endl;

    // STEP 1 - Risoluzione dinamica offsets (pattern scan)
    if (!g_offsets.resolve_all())
    {
        std::cout << "[SkullTelemetry] ERROR: Could not resolve memory offsets!" << std::endl;
        return false;
    }

    // STEP 2 - Creazione Telemetry Manager
    g_manager = std::make_unique<TelemetryManager>();

    if (!g_manager->initialize("127.0.0.1", 5500))
    {
        std::cout << "[SkullTelemetry] ERROR: TelemetryManager initialization failed!" << std::endl;
        g_manager.reset();
        return false;
    }

    // STEP 3 - Avvio worker thread
    g_manager->start();

    std::cout << "[SkullTelemetry] Plugin started successfully." << std::endl;
    return true;
}

void plugin_shutdown()
{
    std::cout << "[SkullTelemetry] Shutdown requested..." << std::endl;

    if (g_manager)
    {
        g_manager->stop();
        g_manager.reset();
    }

    std::cout << "[SkullTelemetry] Shutdown completed." << std::endl;
}
