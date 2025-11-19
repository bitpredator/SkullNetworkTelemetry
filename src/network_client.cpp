#include "network_client.h"

NetworkClient::NetworkClient(const std::string& host, int port)
    : host(host), port(port) {}

NetworkClient::~NetworkClient() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
    }
    WSACleanup();
}

bool NetworkClient::connect_socket() {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "[NetworkClient] WSAStartup failed!" << std::endl;
        return false;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "[NetworkClient] Error creating socket!" << std::endl;
        return false;
    }

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &server.sin_addr) <= 0) {
        std::cerr << "[NetworkClient] Invalid host address!" << std::endl;
        return false;
    }

    if (connect(sock, reinterpret_cast<sockaddr*>(&server), sizeof(server)) < 0) {
        std::cerr << "[NetworkClient] Connection to server failed!" << std::endl;
        return false;
    }

    std::cout << "[NetworkClient] Connected to " << host << ":" << port << std::endl;
    return true;
}

bool NetworkClient::send(const std::string& message) {
    if (sock == INVALID_SOCKET)
        return false;

    int result = ::send(sock, message.c_str(), (int)message.size(), 0);

    if (result == SOCKET_ERROR) {
        std::cerr << "[NetworkClient] Send failed!" << std::endl;
        return false;
    }

    return true;
}
