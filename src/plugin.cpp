#include "plugin.h"
#include "network_client.h"
#include "telemetry_payload.h"

#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <random>

// Se vorrai usare gli header SCS:
// #include "scs_telemetry/scssdk.h"
// #include "scs_telemetry/scssdk_telemetry.h"
// => li userai per registrare i callback e riempire TelemetryPayload con dati reali.

static std::atomic<bool> g_running{ false };
static std::thread g_worker;
static NetworkClient* g_net = nullptr;

// Funzione worker: tenta la connessione e invia payload di prova ogni 2 secondi.
// In seguito qui inseriremo la logica di serializzazione dai callback SCS.
static void worker_thread()
{
    if (!g_net) g_net = new NetworkClient("127.0.0.1", 5500);

    // RNG per demo session id
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;

    std::string session_id;
    {
        std::ostringstream ss;
        ss << std::hex << dist(gen);
        session_id = ss.str();
    }

    while (g_running.load())
    {
        // Assicuriamoci di essere connessi (retry interno)
        bool ok = g_net->ensure_connected();
        if (!ok) {
            // se non connessi, aspettare e riprovare (non bloccare gioco)
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        // Creiamo un payload di esempio (sostituire con dati gioco)
        TelemetryPayload payload;
        payload.session_id = session_id;
        payload.lat = 45.4642; // demo (Milano)
        payload.lon = 9.1900;
        payload.speed_kph = 72.5f;
        payload.heading = 180.0f;
        payload.truck_make = "Scania";
        payload.truck_model = "S730";

        std::string json = payload.to_json();

        bool sent = g_net->send_data(json);
        if (!sent) {
            // rete fallita: chiudiamo e riproviamo
            g_net->close();
        }

        // frequenza di invio demo: 2s (regola secondo esigenze)
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

// Export inizializzazione chiamata dal gioco
extern "C" __declspec(dllexport)
void scs_plugin_init(const unsigned int version, void* api)
{
    // Qui puoi usare 'version' e 'api' per registrare callback SCS.
    // Esempio (pseudocodice):
    // scs_telemetry_register_callback(api, telemetry_callback_fn, user_ptr);

    // Avviamo il worker in background (non-blocking)
    g_running.store(true);
    g_worker = std::thread(worker_thread);

    std::cout << "[SkullNetworkTelemetry] scs_plugin_init called. Worker started.\n";
}

// Export shutdown chiamata dal gioco
extern "C" __declspec(dllexport)
void scs_plugin_shutdown()
{
    g_running.store(false);
    if (g_worker.joinable()) {
        // attendiamo un piccolo timeout per non bloccare il gioco in shutdown
        // join con timeout emulato
        using namespace std::chrono_literals;
        auto start = std::chrono::steady_clock::now();
        while (g_worker.joinable()) {
            std::this_thread::sleep_for(10ms);
            if (std::chrono::steady_clock::now() - start > 500ms) {
                // forziamo detach per non bloccare il processo del gioco
                try {
                    g_worker.detach();
                } catch (...) { }
                break;
            }
        }
    }

    if (g_net) {
        g_net->close();
        delete g_net;
        g_net = nullptr;
    }

    std::cout << "[SkullNetworkTelemetry] scs_plugin_shutdown called.\n";
}

void plugin_shutdown()
{
    // wrapper chiamato da DllMain
    scs_plugin_shutdown();
}
