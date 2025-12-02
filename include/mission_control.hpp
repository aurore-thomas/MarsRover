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
    void UpdateMapWithDiscoveredTiles(Tile **map, const int x, const int y, const ObjectType type);
    
    public:
    MissionControl(unsigned short port);
    void Main() override;
    
    void DisplayMap(int width, int height, Tile** map, Orientation orientation) const;
    ObjectType StringToObjectType(const string &typeStr);
    
    bool HasUnknownTiles(Tile **map, int width, int height) const;
    bool IsValidCommand(const string &command);
  };
  
  
