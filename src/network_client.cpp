#include "network_client.h"
#include <iostream>
#include <chrono>
#include <thread>

NetworkClient::NetworkClient(const std::string& host, unsigned short port)
    : m_host(host), m_port(port), m_sock(INVALID_SOCKET)
{
    // inizializzazione WSA
    WSADATA wsa;
    int res = WSAStartup(MAKEWORD(2,2), &wsa);
    if (res != 0) {
        std::cerr << "[SkullNetworkTelemetry] WSAStartup failed: " << res << "\n";
    }
}

NetworkClient::~NetworkClient() {
    close();
    WSACleanup();
}

bool NetworkClient::create_and_connect_socket() {
    cleanup_socket();

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port);

    if (inet_pton(AF_INET, m_host.c_str(), &addr.sin_addr) != 1) {
        closesocket(s);
        return false;
    }

    // set non-blocking temporarily for connect with timeout
    u_long mode = 1;
    ioctlsocket(s, FIONBIO, &mode);

    int rc = connect(s, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (rc == SOCKET_ERROR) {
        int last = WSAGetLastError();
        if (last != WSAEWOULDBLOCK && last != WSAEINPROGRESS) {
            closesocket(s);
            return false;
        }
    }

    // wait for socket writable or timeout (2s)
    fd_set writeSet;
    FD_ZERO(&writeSet);
    FD_SET(s, &writeSet);
    timeval tv{};
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    int sel = select(0, nullptr, &writeSet, nullptr, &tv);
    if (sel > 0 && FD_ISSET(s, &writeSet)) {
        // set blocking again
        mode = 0;
        ioctlsocket(s, FIONBIO, &mode);
        m_sock = s;
        return true;
    } else {
        closesocket(s);
        return false;
    }
}

bool NetworkClient::ensure_connected() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_sock != INVALID_SOCKET) {
        return true;
    }
    return create_and_connect_socket();
}

bool NetworkClient::send_data(const std::string& data) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_sock == INVALID_SOCKET) {
        if (!create_and_connect_socket())
            return false;
    }

    // send length-prefixed or raw? Qui inviamo raw + newline
    std::string payload = data + "\n";
    const char* ptr = payload.c_str();
    size_t remaining = payload.size();

    while (remaining > 0) {
        int sent = ::send(m_sock, ptr, static_cast<int>(remaining), 0);
        if (sent == SOCKET_ERROR) {
            int err = WSAGetLastError();
            std::cerr << "[SkullNetworkTelemetry] send failed: " << err << "\n";
            cleanup_socket();
            return false;
        }
        ptr += sent;
        remaining -= sent;
    }
    return true;
}

void NetworkClient::cleanup_socket() {
    if (m_sock != INVALID_SOCKET) {
        closesocket(m_sock);
        m_sock = INVALID_SOCKET;
    }
}

void NetworkClient::close() {
    std::lock_guard<std::mutex> lock(m_mutex);
    cleanup_socket();
}
