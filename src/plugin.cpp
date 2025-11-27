#include "plugin.h"
#include "logger.h"
#include "network_client.h"
#include "telemetry_manager.h"

// =======================================================
//  Funzioni richieste dal motore di ETS2/ATS
// =======================================================
extern "C" {

// -----------------------------------------------------------
// Quando ETS2 carica il plugin
// -----------------------------------------------------------
SCSAPI_RESULT scs_telemetry_init(const scs_telemetry_init_params_t *params)
{
    Logger::info("[PLUGIN] scs_telemetry_init() chiamato");

    // Connessione al server TCP
    Logger::info("[PLUGIN] Connessione al server TCP 127.0.0.1:5500...");
    NetworkClient::instance().connect_to_server("127.0.0.1", 5500);

    // Avvio manager telemetria
    Logger::info("[PLUGIN] Avvio TelemetryManager...");
    TelemetryManager::instance().start();

    Logger::info("[PLUGIN] Plugin inizializzato correttamente");
    return SCS_RESULT_ok;
}

// -----------------------------------------------------------
// Quando ETS2 chiude o scarica il plugin
// -----------------------------------------------------------
void scs_telemetry_shutdown()
{
    Logger::info("[PLUGIN] scs_telemetry_shutdown() chiamato");

    TelemetryManager::instance().stop();
    NetworkClient::instance().disconnect();

    Logger::info("[PLUGIN] Plugin terminato");
}

} // extern "C"

// =======================================================
//  Funzione interna per DllMain
// =======================================================
void plugin_shutdown()
{
    Logger::info("[PLUGIN] plugin_shutdown() chiamato da DllMain");
    scs_telemetry_shutdown();
}
