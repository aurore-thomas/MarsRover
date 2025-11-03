#pragma once

#include <string>

#include "packet.hpp"

using namespace std;

class ISocket {
  public:

    virtual bool Init() = 0;
    virtual void Cleanup() = 0;
    
    virtual bool Create() = 0;
    virtual bool Bind(unsigned short port) = 0;
    virtual bool Listen(int backlog) = 0;
    virtual bool Connect(const std::string& host, unsigned short port) = 0;
    virtual int  Accept() = 0;
    virtual bool Send(Packet& packet) = 0;
    virtual bool Receive(Packet& packet) = 0;
    virtual void Close() = 0;
};
