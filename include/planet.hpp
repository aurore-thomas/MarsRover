#pragma once
#include <list>

#include "iplanet.hpp"

using namespace std;

struct Tile {
    int x;
    int y;
    ObjectType type;
};

class Planet : public IPlanet{

  friend class PlanetUnitTest;
  friend class PlanetClassTest;
  friend class PlanetIntegrationTest;

  friend class Rover;
  friend class MissionControl;
  private:
    int width;
    int height;
    Tile** map;

    Tile** createMap(int width, int height);
    Tile** createMapMissionControl(int width, int height);
    bool IsFreeTile(int x, int y) const override;

  public:
  
    Planet(int width, int height, bool isMissionControl = false);
    int getWidth() const;
    int getHeight() const;
    Tile** getMap() const;
    
};
