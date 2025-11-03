#pragma once
#include <vector>
#include <cstdint>
#include <string>

enum Orientation {NORTH, EAST, SOUTH, WEST};
enum Command {MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT};

class Packet {
    private:
        std::string listInstructions;
        int positionX;
        int positionY;
        bool obstacle;
        Orientation orientation;
    
    public:
        std::vector<uint8_t> SerializePacket();
        Packet DeserializePacket(const std::vector<uint8_t>& buffer);

        std::string getListInstructions() const;
        void setListInstructions(const std::string& listInstructions);
        int getPacketPositionX() const;
        void setPacketPositionX(int positionX);
        int getPacketPositionY() const;
        void setPacketPositionY(int positionY);
        bool getPacketObstacle() const;
        void setPacketObstacle(bool obstacle);
        Orientation getPacketOrientation() const;
        void setPacketOrientation(Orientation orientation);
};
