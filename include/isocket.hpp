#pragma once

#include <string>

#include "packet.hpp"

using namespace std;

class ISocket {
  public:    
    virtual bool Send(Packet& packet) = 0;
    virtual bool Receive(Packet& packet) = 0;
};
