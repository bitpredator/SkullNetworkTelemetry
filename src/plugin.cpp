#include "plugin.h"
#include "network_client.h"
#include "telemetry_manager.h"

// =======================================================
//  Funzioni richieste dal motore di ETS2/ATS
//  (devono essere esportate in C linkage)
// =======================================================
extern "C" {

// ---------------------------------------------------------------------------
// Called when ETS2 loads the plugin
// ---------------------------------------------------------------------------
SCSAPI_RESULT scs_telemetry_init(const scs_telemetry_init_params_t *params)
{
    NetworkClient::instance().connect_to_server("127.0.0.1", 5500);
    TelemetryManager::instance().start();

    return SCS_RESULT_ok;
}

// ---------------------------------------------------------------------------
// Called when ETS2 unloads the plugin
// ---------------------------------------------------------------------------
SCSAPI_VOID scs_telemetry_shutdown()
{
    TelemetryManager::instance().stop();
    NetworkClient::instance().disconnect();
}

} // extern "C"


// =======================================================
//  Funzione interna, chiamata da DllMain
//  (serve per evitare dipendenze da extern "C")
// =======================================================
void plugin_shutdown()
{
    // Chiama la shutdown della telemetria
    scs_telemetry_shutdown();
}
