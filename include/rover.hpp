#pragma once

#include <string>

#include "common/unix_socket.hpp"
#include "planet.hpp"

using namespace std;

class Rover { 
  private:
    Orientation orientation;
    int positionX;
    int positionY;

    void InitializeRoverPosition(Planet &planet);

    void RoverMovement(Rover &rover, Planet &planet, Packet &response, int multiplicator);
    Packet ExecuteCommand(const string& command, Rover &rover, Planet &planet);

    Orientation RotationHoraire(Orientation firstOrientation);
    Orientation RotationAntiHoraire(Orientation firstOrientation);

    
  public:
    Rover(Planet &planet);

    Orientation getOrientation() const;
    void setOrientation(Orientation newOrientation);
    void setPositionX(const int x);
    int getPositionX() const;
    void setPositionY(const int y);
    int getPositionY() const;
};

