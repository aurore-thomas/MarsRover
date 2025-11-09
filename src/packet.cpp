#include "packet.hpp"

#include <cstring>
#include <arpa/inet.h> 

static constexpr uint8_t TAG_ROVER = 0x01;
static constexpr uint8_t TAG_MISSION = 0x02;

void Packet::writeUint32(std::vector<uint8_t>& buf, uint32_t v) {
    uint32_t n = htonl(v);
    buf.insert(buf.end(), reinterpret_cast<uint8_t*>(&n), reinterpret_cast<uint8_t*>(&n) + sizeof(n));
}

uint32_t Packet::readUint32(const std::vector<uint8_t>& buf, size_t& off) {
    if (off + sizeof(uint32_t) > buf.size())
        throw std::string("Invalid packet data (uint32 length)");
    uint32_t n = 0;
    std::memcpy(&n, buf.data() + off, sizeof(n));
    off += sizeof(n);
    return ntohl(n);
}

void Packet::writeString(std::vector<uint8_t>& buf, const std::string& s) {
    writeUint32(buf, static_cast<uint32_t>(s.size()));
    buf.insert(buf.end(), s.begin(), s.end());
}

std::string Packet::readString(const std::vector<uint8_t>& buf, size_t& off) {
    uint32_t len = readUint32(buf, off);
    if (off + len > buf.size())
        throw std::string("Invalid packet data (string bytes)");
    std::string s(reinterpret_cast<const char*>(buf.data() + off), len);
    off += len;
    return s;
}

std::vector<uint8_t> Packet::SerializePacket() {
    std::vector<uint8_t> buf;

    if (missionControlPacket.finished || !missionControlPacket.listInstructions.empty()) 
    {
        buf.push_back(TAG_MISSION);
        buf.push_back(missionControlPacket.finished ? 1 : 0);
        writeString(buf, missionControlPacket.listInstructions);
        return buf;
    }
    else 
    {
        buf.push_back(TAG_ROVER);
        writeUint32(buf, static_cast<uint32_t>(roverPacket.roverX));
        writeUint32(buf, static_cast<uint32_t>(roverPacket.roverY));
        writeUint32(buf, static_cast<uint32_t>(roverPacket.orientation));
        writeUint32(buf, static_cast<uint32_t>(roverPacket.planetWidth));
        writeUint32(buf, static_cast<uint32_t>(roverPacket.planetHeight));

        writeUint32(buf, static_cast<uint32_t>(roverPacket.tilesDiscovered.size()));
        for (const auto& t : roverPacket.tilesDiscovered) {
            writeUint32(buf, static_cast<uint32_t>(t.x));
            writeUint32(buf, static_cast<uint32_t>(t.y));
            writeString(buf, t.type);
        }
    }
    return buf;
}

Packet Packet::DeserializePacket(const std::vector<uint8_t>& buffer) {
    Packet out;
    if (buffer.empty())
        throw std::string("Empty packet buffer");

    size_t off = 0;
    uint8_t tag = buffer[off++];

    if (tag == TAG_MISSION) {
        if (off >= buffer.size())
            throw std::string("Invalid mission packet (missing finished flag)");
        MissionControlPacket m;
        m.finished = (buffer[off++] != 0);
        m.listInstructions = readString(buffer, off);
        out.missionControlPacket = std::move(m);
        return out;
    }

    if (tag == TAG_ROVER) {
        RoverPacket r;
        r.roverX = static_cast<int>(readUint32(buffer, off));
        r.roverY = static_cast<int>(readUint32(buffer, off));
        r.orientation = static_cast<int>(readUint32(buffer, off));
        r.planetWidth = static_cast<int>(readUint32(buffer, off));
        r.planetHeight = static_cast<int>(readUint32(buffer, off));

        uint32_t tilesCount = readUint32(buffer, off);
        r.tilesDiscovered.reserve(tilesCount);
        for (uint32_t i = 0; i < tilesCount; ++i) {
            RoverPacket::TileDiscovered t;
            t.x = static_cast<int>(readUint32(buffer, off));
            t.y = static_cast<int>(readUint32(buffer, off));
            t.type = readString(buffer, off);
            r.tilesDiscovered.push_back(std::move(t));
        }

        out.roverPacket = std::move(r);
        return out;
    }

    throw std::string("Unknown packet tag");
}
