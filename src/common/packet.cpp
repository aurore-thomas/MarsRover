#include "common/packet.hpp"

#include <vector>
#include <string>
#include <cstring>
#include <cstdint>

std::vector<uint8_t> Packet::SerializePacket() {
    std::vector<uint8_t> buffer;

    auto writeString = [&](const std::string& str) {
        uint32_t len = static_cast<uint32_t>(str.size());
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&len), reinterpret_cast<uint8_t*>(&len) + sizeof(len));
        buffer.insert(buffer.end(), str.begin(), str.end());
    };

    writeString(listInstructions);

    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&positionX),
                  reinterpret_cast<const uint8_t*>(&positionX) + sizeof(positionX));
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&positionY),
                  reinterpret_cast<const uint8_t*>(&positionY) + sizeof(positionY));
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&obstacle),
                  reinterpret_cast<const uint8_t*>(&obstacle) + sizeof(obstacle));

    writeString(reinterpret_cast<const char*>(&orientation));

    return buffer;
}

Packet Packet::DeserializePacket(const std::vector<uint8_t>& buffer) {
    Packet p;
    size_t offset = 0;

    auto readString = [&](std::string& str) {
        if (offset + sizeof(uint32_t) > buffer.size())
            throw std::string("Invalid packet data (string length)");

        uint32_t len;
        std::memcpy(&len, buffer.data() + offset, sizeof(len));
        offset += sizeof(len);

        if (offset + len > buffer.size())
            throw std::string("Invalid packet data (string data)");

        str.assign(reinterpret_cast<const char*>(buffer.data() + offset), len);
        offset += len;
    };

    readString(p.listInstructions);

    std::memcpy(&p.positionX, buffer.data() + offset, sizeof(p.positionX));
    offset += sizeof(p.positionX);

    std::memcpy(&p.positionY, buffer.data() + offset, sizeof(p.positionY));
    offset += sizeof(p.positionY);

    std::memcpy(&p.obstacle, buffer.data() + offset, sizeof(p.obstacle));
    offset += sizeof(p.obstacle);

    std::memcpy(&p.orientation, buffer.data() + offset, sizeof(p.orientation));
    offset += sizeof(p.orientation);

    return p;
}

std::string Packet::getListInstructions() const {
    return listInstructions;
}
void Packet::setListInstructions(const std::string& listInstructions) {
    this->listInstructions = listInstructions;
}

int Packet::getPacketPositionX() const {
    return positionX;
}
void Packet::setPacketPositionX(int positionX) {
    this->positionX = positionX;
}

int Packet::getPacketPositionY() const {
    return positionY;
}
void Packet::setPacketPositionY(int positionY) {
    this->positionY = positionY;
}

bool Packet::getPacketObstacle() const {
    return obstacle;
}

void Packet::setPacketObstacle(bool obstacle) {
    this->obstacle = obstacle;
}

Orientation Packet::getPacketOrientation() const {
    return orientation;
}

void Packet::setPacketOrientation(Orientation orientation) {
    this->orientation = orientation;
}
