#pragma once

#ifndef _UNIX_
#include "isocket.hpp"
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

class UnixSocket : public ISocket {
    int sock = -1;

public: 
    UnixSocket();
    ~UnixSocket();

    bool Send(Packet& packet) override;
    bool Receive(Packet& packet) override;

    void setSock(int socket);
    int getSock() const;
};
#endif
