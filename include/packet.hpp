#pragma once
#include <vector>
#include <cstdint>
#include <string>

struct RoverPacket {
    struct TileDiscovered {
        int x;
        int y;
        std::string type;
    };

    int roverX;
    int roverY;
    int orientation;
    int planetWidth;
    int planetHeight;
    std::vector<TileDiscovered> tilesDiscovered;
};

struct MissionControlPacket {
    bool finished;
    std::string listInstructions;
};

class Packet {
    private:
        RoverPacket roverPacket;
        MissionControlPacket missionControlPacket;

        static void writeUint32(std::vector<uint8_t>& buf, uint32_t v);
        static uint32_t readUint32(const std::vector<uint8_t>& buf, size_t& off);
        static void writeString(std::vector<uint8_t>& buf, const std::string& s);
        static std::string readString(const std::vector<uint8_t>& buf, size_t& off);

    public:
        std::vector<uint8_t> SerializePacket();
        Packet DeserializePacket(const std::vector<uint8_t>& buffer);

        void setRoverPacket(const RoverPacket& p) { roverPacket = p; }
        RoverPacket getRoverPacket() const { return roverPacket; }
        void setMissionControlPacket(const MissionControlPacket& p) { missionControlPacket = p; }
        MissionControlPacket getMissionControlPacket() const { return missionControlPacket; }
};
