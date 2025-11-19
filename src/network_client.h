#pragma once
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

class NetworkClient {
public:
    NetworkClient(const std::string& host, int port);
    ~NetworkClient();

    bool connect_socket();
    bool send(const std::string& message);

private:
    std::string host;
    int port;
    SOCKET sock = INVALID_SOCKET;
};
