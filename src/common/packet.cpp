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
    int32_t px = static_cast<int32_t>(positionX);
    int32_t py = static_cast<int32_t>(positionY);
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&px), reinterpret_cast<const uint8_t*>(&px) + sizeof(px));
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&py), reinterpret_cast<const uint8_t*>(&py) + sizeof(py));

    uint8_t obs = obstacle ? 1 : 0;
    buffer.push_back(obs);

    int32_t orient = static_cast<int32_t>(orientation);
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&orient), reinterpret_cast<const uint8_t*>(&orient) + sizeof(orient));

    int32_t w = static_cast<int32_t>(planetWidth);
    int32_t h = static_cast<int32_t>(planetHeight);
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&w), reinterpret_cast<const uint8_t*>(&w) + sizeof(w));
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&h), reinterpret_cast<const uint8_t*>(&h) + sizeof(h));

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
    if (offset + sizeof(int32_t) * 2 > buffer.size())
        throw std::string("Invalid packet data (positions)");
    int32_t px = 0, py = 0;
    std::memcpy(&px, buffer.data() + offset, sizeof(px));
    offset += sizeof(px);
    std::memcpy(&py, buffer.data() + offset, sizeof(py));
    offset += sizeof(py);
    p.positionX = static_cast<int>(px);
    p.positionY = static_cast<int>(py);

    if (offset + 1 > buffer.size())
        throw std::string("Invalid packet data (obstacle)");
    uint8_t obs = buffer[offset];
    offset += 1;
    p.obstacle = (obs != 0);

    if (offset + sizeof(int32_t) > buffer.size())
        throw std::string("Invalid packet data (orientation)");
    int32_t orient = 0;
    std::memcpy(&orient, buffer.data() + offset, sizeof(orient));
    offset += sizeof(orient);
    p.orientation = static_cast<Orientation>(orient);

    if (offset + sizeof(int32_t) * 2 > buffer.size())
        throw std::string("Invalid packet data (planet dims)");
    int32_t w = 0, h = 0;
    std::memcpy(&w, buffer.data() + offset, sizeof(w));
    offset += sizeof(w);
    std::memcpy(&h, buffer.data() + offset, sizeof(h));
    offset += sizeof(h);
    p.planetWidth = static_cast<int>(w);
    p.planetHeight = static_cast<int>(h);

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

int Packet::getPacketPlanetWidth() const {
    return planetWidth;
}

void Packet::setPacketPlanetWidth(int planetWidth) {
    this->planetWidth = planetWidth;
}

int Packet::getPacketPlanetHeight() const {
    return planetHeight;
}

void Packet::setPacketPlanetHeight(int planetHeight) {
    this->planetHeight = planetHeight;
}
