#include "network_client.h"

#include <iostream>

bool NetworkClient::connect_to_server(const std::string& host, int port) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) return false;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) return false;

    sockaddr_in srv{};
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &srv.sin_addr);

    if (connect(sock, (sockaddr*)&srv, sizeof(srv)) == SOCKET_ERROR) {
        closesocket(sock);
        sock = INVALID_SOCKET;
        WSACleanup();
        return false;
    }

    connected = true;
    return true;
}

bool NetworkClient::send_data(const std::string& data) {
    if (!connected || sock == INVALID_SOCKET) return false;
    int sent = send(sock, data.c_str(), (int)data.size(), 0);
    return sent == (int)data.size();
}

void NetworkClient::disconnect() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    connected = false;
    WSACleanup();
}
