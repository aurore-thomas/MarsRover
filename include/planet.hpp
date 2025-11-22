#pragma once
#include <list>

#include "iplanet.hpp"

using namespace std;

struct Tile {
    int x;
    int y;
    ObjectType type;
};

class Planet : public IPlanet
{
  private:
    int width;
    int height;
    Tile** map;

    Tile** createMap(int width, int height);
    Tile** createMapMissionControl(int width, int height);

  public:
    Planet(int width, int height, bool isMissionControl = false);

    int getWidth() const;
    int getHeight() const;
    Tile** getMap() const;
    void setMap(Tile** map);

    bool IsFreeTile(int x, int y) const;
};
