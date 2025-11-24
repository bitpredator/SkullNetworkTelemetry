#pragma once
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class NetworkClient {
public:
    static NetworkClient& instance() {
        static NetworkClient inst;
        return inst;
    }

    bool connect_to_server(const std::string& host, int port);
    void disconnect();
    bool send_data(const std::string& data);
    bool is_connected() const { return connected; }

private:
    NetworkClient() = default;
    ~NetworkClient() = default;

    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;

    SOCKET sock = INVALID_SOCKET;
    bool connected = false;
};
