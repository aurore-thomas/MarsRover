#pragma once

#include "imission_control.hpp"
#include "unix_socket.hpp"
#include "planet.hpp"

using namespace std;

class MissionControl : public IMissionControl
{
  private:
    unsigned short port;
    UnixSocket server;
    bool LaunchServer();
    bool Bind(unsigned short port);
    int Accept();

    string AskCommand();
    bool IsValidCommand(const string &command);
    ObjectType StringToObjectType(const string &typeStr);
    void DisplayMap(int width, int height, Tile** map, Orientation orientation) const;

  public:
    MissionControl(unsigned short port);
    void Main() override;
};


