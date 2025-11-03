#pragma once

#include <string>

#include "common/communication.hpp"
#include "planet.hpp"

using namespace std;

class Rover { 
  private:
    Orientation orientation;
    int positionX;
    int positionY;

    void RoverMovement(Rover &rover, Planet &planet, Packet &response, int multiplicator);
    
    public:
      Rover(int x, int y, Orientation orientation);

      Packet ExecuteCommand(const string& command, Rover &rover, Planet &planet);

      Orientation getOrientation() const;
      void setOrientation(Orientation newOrientation);
      void setPositionX(const int x);
      int getPositionX() const;
      void setPositionY(const int y);
      int getPositionY() const;
};

