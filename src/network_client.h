#pragma once
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class NetworkClient {
public:
    // Singleton accessor
    static NetworkClient& instance() {
        static NetworkClient client;
        return client;
    }

    bool connect_to_server(const std::string& host, int port);
    bool send_data(const std::string& data);
    void disconnect();

    bool is_connected() const { return connected; }

private:
    NetworkClient() = default; // costruttore privato
    ~NetworkClient() = default;

    // vietiamo copia e assegnazione
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;

    SOCKET sock = INVALID_SOCKET;
    bool connected = false;
};
