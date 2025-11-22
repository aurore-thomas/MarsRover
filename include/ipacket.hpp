#pragma once
#include <vector>
#include <cstdint>

// Forward-declare Packet to avoid a circular include. The concrete
// definition of Packet lives in `packet.hpp` which includes this file.
class Packet;

class IPacket
{
  public:
    virtual std::vector<uint8_t> SerializePacket() = 0;
    virtual Packet DeserializePacket(const std::vector<uint8_t>& buffer) = 0;
};