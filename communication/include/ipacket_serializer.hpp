#pragma once

#include <vector>
#include <cstdint>

// Forward-declare PacketSerializer to avoid a circular include. The concrete
// definition of PacketSerializer lives in `packet_serializer.hpp` which includes this file.
class PacketSerializer;

class IPacketSerializer 
{
  public:
    virtual std::vector<uint8_t> SerializePacket() = 0;
    virtual PacketSerializer DeserializePacket(const std::vector<uint8_t>& buffer) = 0;
};
