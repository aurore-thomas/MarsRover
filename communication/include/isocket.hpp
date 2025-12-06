#pragma once

#include <string>

#include "packet_serializer.hpp"

using namespace std;

class ISocket {
  public:    
    virtual bool Send(PacketSerializer& packet) = 0;
    virtual bool Receive(PacketSerializer& packet) = 0;
};
