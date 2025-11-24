#include "network_client.h"

bool NetworkClient::connect_to_server(const std::string& host, int port) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        return false;

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, host.c_str(), &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        closesocket(sock);
        sock = INVALID_SOCKET;
        return false;
    }

    connected = true;
    return true;
}

void NetworkClient::disconnect() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    connected = false;
    WSACleanup();
}

bool NetworkClient::send_data(const std::string& data) {
    if (!connected) return false;
    int sent = send(sock, data.c_str(), data.size(), 0);
    return sent == (int)data.size();
}
