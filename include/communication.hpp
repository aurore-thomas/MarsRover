#pragma once

#include <string>
#include <vector.h>
#include "packet.hpp"

using namespace std;

enum Orientation {NORTH, EAST, SOUTH, WEST};
enum Command {MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT};

struct Response {
    int positionX;
    int positionY;
    bool obstacle;
    string orientation;
};

class Communication {
  protected:
    Communication(const std::string& text);

    void Listen() const;
    int Send(const Packet& packet) const;
  
    virtual void DecodeResponse(const Packet& packet) const; 
    virtual Packet EncodeRequest() const;
};