#pragma once

#include <string>

#include "unix_socket.hpp"
#include "planet.hpp"

using namespace std;

class Rover
{
  friend class RoverUnitTest;
  friend class RoverClassTest;
  friend class RoverIntegrationTest;  
  
  private:
    UnixSocket client;
    Planet& planet;
    int positionX;
    int positionY;
    Orientation orientation;

    bool LaunchClient(const unsigned short port, string address);
    void InitializeRoverPosition();  
    bool RoverMovement(RoverPacket &response, int multiplicator);
    Command ConvertCharToCommand(char commandChar);
    RoverPacket ExecuteCommand(const string &command);
    Orientation RotationHoraire(Orientation firstOrientation);
    Orientation RotationAntiHoraire(Orientation firstOrientation);
    int Modulo(int a, int b);
  
  public:
    Rover(Planet &planet, const unsigned short port, string address);
    
    void Main();
    
    int getX() const { return positionX; };
    int getY() const { return positionY; };
    Orientation getOrientation() const { return orientation; };
};
