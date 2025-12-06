#pragma once

#include "unix_socket.hpp"
#include "planet.hpp"

using namespace std;

class MissionControl
{
  friend class MissionControlUnitTest;
  friend class MissionControlClassTest;
  friend class MissionControlIntegrationTest;

  private:
    unsigned short port;
    UnixSocket server;
    
    bool LaunchServer();
    bool Bind(unsigned short port);
    int Accept();

    string AskCommand();
    void UpdateMapWithDiscoveredTiles(Tile **map, const int x, const int y, const ObjectType type);
    void DisplayMap(int width, int height, Tile** map, Orientation orientation) const;
    bool HasUnknownTiles(Tile **map, int width, int height) const;
    bool IsValidCommand(const string &command);
    ObjectType StringToObjectType(const string &typeStr);
    
  public:
    void Main();
    MissionControl(unsigned short port);
  };
