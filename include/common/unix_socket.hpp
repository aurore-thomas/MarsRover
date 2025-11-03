#pragma once

#ifndef _UNIX_
#include "communication.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

class UnixSocket : public ISocket {
    int sock = -1;

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
    void Close() override;

};
#endif
