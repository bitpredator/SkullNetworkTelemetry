#include "network_client.h"
#include <iostream>

bool NetworkClient::connect_to_server(const std::string& host, int port) {
    WSADATA wsaData;

    std::cout << "[Telemetry] Starting WSA..." << std::endl;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "[Telemetry] WSAStartup failed" << std::endl;
        return false;
    }

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cout << "[Telemetry] Socket creation failed" << std::endl;
        return false;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &server.sin_addr) <= 0) {
        std::cout << "[Telemetry] Invalid IP address" << std::endl;
        closesocket(sock);
        sock = INVALID_SOCKET;
        return false;
    }

    std::cout << "[Telemetry] Connecting to " << host << ":" << port << "..." << std::endl;

    if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cout << "[Telemetry] Connection failed!" << std::endl;
        closesocket(sock);
        sock = INVALID_SOCKET;
        return false;
    }

    std::cout << "[Telemetry] Connected successfully!" << std::endl;

    connected = true;
    return true;
}

void NetworkClient::disconnect() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    connected = false;
    std::cout << "[Telemetry] Disconnected" << std::endl;
    WSACleanup();
}

bool NetworkClient::send_data(const std::string& data) {
    if (!connected) {
        std::cout << "[Telemetry] NOT CONNECTED – cannot send" << std::endl;
        return false;
    }

    std::cout << "[Telemetry] Sending: " << data << std::endl;

    int sent = send(sock, data.c_str(), (int)data.size(), 0);

    if (sent != (int)data.size()) {
        std::cout << "[Telemetry] Send failed!" << std::endl;
        return false;
    }

    std::cout << "[Telemetry] Send OK" << std::endl;
    return true;
}
