#include "plugin.h"
#include "logger.h"
#include "network_client.h"
#include "telemetry_manager.h"

extern "C" {

// Called by game on plugin load
scs_result_t SCSAPI_CALL scs_telemetry_init(const scs_telemetry_init_params_t* params)
{
    Logger::info("[PLUGIN] scs_telemetry_init() called");

    if (!params) {
        Logger::error("[PLUGIN] params == nullptr");
        return SCS_RESULT_generic_error;
    }

    // Try connect to backend (best-effort)
    Logger::info("[PLUGIN] Connecting to telemetry server 127.0.0.1:5500");
    NetworkClient::instance().connect_to_server("127.0.0.1", 5500);

    // Initialize TelemetryManager with SDK params (register channels if provided)
    if (!TelemetryManager::instance().initialize(params)) {
        Logger::error("[PLUGIN] TelemetryManager::initialize failed");
        // still return ok so plugin remains loaded? Here we return ok but you can choose generic_error
        // return SCS_RESULT_generic_error;
    }

    // start background worker that sends snapshots to server
    TelemetryManager::instance().start();

    Logger::info("[PLUGIN] Initialized");
    return SCS_RESULT_ok;
}

// Called by game on plugin unload
void SCSAPI_CALL scs_telemetry_shutdown(void)
{
    Logger::info("[PLUGIN] scs_telemetry_shutdown() called");
    TelemetryManager::instance().stop();
    NetworkClient::instance().disconnect();
    Logger::info("[PLUGIN] Shutdown completed");
}

} // extern "C"

// Called by DllMain only (safe wrapper)
void plugin_shutdown()
{
    Logger::info("[PLUGIN] plugin_shutdown() called from DllMain");
    scs_telemetry_shutdown();
}
