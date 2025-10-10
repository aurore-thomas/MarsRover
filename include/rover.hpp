#pragma once

#include <string>

#include "communication.hpp"
#include "planet.hpp"

using namespace std;

class Rover : public Communication{
  private:
    Orientation orientation;
    int positionX;
    int positionY;

    void RoverMovement(Rover &rover, Planet &planet, Response &response, int multiplicator);
    
    public:
    Rover(int x, int y, Orientation orientation);
    Response ExecuteCommand(const string& command, Rover &rover, Planet &planet);

    Orientation getOrientation() const;
    void setOrientation(Orientation newOrientation);
    
    void setPositionX(const int x);
    int getPositionX() const;

    void setPositionY(const int y);
    int getPositionY() const;
    //string DecodeResponse(const Packet& packet) const override;
    // Packet EncodeRequest() const override;

};

