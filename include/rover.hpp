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
    Orientation orientation;
    int positionX;
    int positionY;

    bool LaunchClient(const unsigned short port, string address);
    void InitializeRoverPosition();

    Orientation RotationHoraire(Orientation firstOrientation);
    Orientation RotationAntiHoraire(Orientation firstOrientation);
    int Modulo(int a, int b);
    bool RoverMovement(RoverPacket &response, int multiplicator);
    Command ConvertCharToCommand(char commandChar);
    RoverPacket ExecuteCommand(const string &command);

  public:
    Rover(Planet &planet, const unsigned short port, string address);
    void Main() override;
};
