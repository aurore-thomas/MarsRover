#pragma once

#ifdef _WIN32
#include "communication.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class WinSocket : public ISocket {
    SOCKET sock = INVALID_SOCKET;

public:
    bool Init() override;
    void Cleanup() override;
    bool Create() override;
    bool Bind(unsigned short port) override;
    bool Listen(int backlog = 5) override;
    int Accept() override;
    bool Connect(const std::string& host, unsigned short port) override;
    bool Send(Packet& packet) override;
    bool Receive(Packet& packet) override;
};
#endif
