#pragma once

#include <string>

#include "irover.hpp"
#include "unix_socket.hpp"
#include "planet.hpp"

using namespace std;

class Rover : public IRover{ 
  private:
  UnixSocket client;
  Planet& planet;
  int positionX;
  int positionY;
  bool LaunchClient(const unsigned short port, string address);
  void InitializeRoverPosition();  
  bool RoverMovement(RoverPacket &response, int multiplicator);
  Command ConvertCharToCommand(char commandChar);
  
  
  Orientation orientation;
  
  public:
  Rover(Planet &planet, const unsigned short port, string address);
  
  void Main() override;
  
  int getX() const { return positionX; };
  int getY() const { return positionY; };
  Orientation getOrientation() const { return orientation; };
  Orientation RotationHoraire(Orientation firstOrientation);
  Orientation RotationAntiHoraire(Orientation firstOrientation);
  
  int Modulo(int a, int b);
  RoverPacket ExecuteCommand(const string &command);
  
};
