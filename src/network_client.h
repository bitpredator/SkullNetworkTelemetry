#pragma once
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mutex>

#pragma comment(lib, "Ws2_32.lib")

class NetworkClient {
public:
    NetworkClient(const std::string& host = "127.0.0.1", unsigned short port = 5500);
    ~NetworkClient();

    // Non copyable
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;

    // Try to ensure connected (non-blocking long wait)
    bool ensure_connected();

    // Send raw data (returns true if at least attempted and send succeeded)
    bool send_data(const std::string& data);

    // Close connection
    void close();

private:
    std::string m_host;
    unsigned short m_port;
    SOCKET m_sock;
    std::mutex m_mutex;

    bool create_and_connect_socket();
    void cleanup_socket();
};
