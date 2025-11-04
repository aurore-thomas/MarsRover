#pragma once
#include <vector>
#include <cstdint>
#include <string>

enum Orientation {NORTH, EAST, SOUTH, WEST};
enum Command {MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT};

struct TileDiscovered {
    int x;
    int y;
    std::string type;
};

class Packet {
    private:
        std::string listInstructions;
        int roverX;
        int roverY;
        Orientation orientation;
        int planetWidth;
        int planetHeight;
        std::vector<TileDiscovered> tilesDiscovered;
        bool finished;
    
    public:
        std::vector<uint8_t> SerializePacket();
        Packet DeserializePacket(const std::vector<uint8_t>& buffer);

        std::string getListInstructions() const;
        void setListInstructions(const std::string& listInstructions);
        int getPacketRoverX() const;
        void setPacketRoverX(int roverX);
        int getPacketRoverY() const;
        void setPacketRoverY(int roverY);
        Orientation getPacketOrientation() const;
        void setPacketOrientation(Orientation orientation);
        int getPacketPlanetWidth() const;
        void setPacketPlanetWidth(int planetWidth);
        int getPacketPlanetHeight() const;
        void setPacketPlanetHeight(int planetHeight);
        const std::vector<TileDiscovered>& getTilesDiscovered() const;
        void addTileDiscovered(int x, int y, const std::string& type);
        bool isFinished() const;
        void setFinished(bool finished);
};
