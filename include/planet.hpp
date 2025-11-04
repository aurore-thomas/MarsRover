#pragma once
#include <list>

using namespace std;

enum ObjectType {EMPTY, ROVER, OBSTACLE, UNKNOWN}; 

struct Tile {
    int x;
    int y;
    ObjectType type;
    bool discovered;
};

class Planet {
  private:
    int width;
    int height;
    Tile** map;

  public:
    Planet(int width, int height);

    int getWidth() const;
    void setWidth(int width);
    int getHeight() const;
    void setHeight(int height);
    Tile** getMap() const;
    void setMap(Tile** map);

    bool IsFreeTile(int x, int y) const;
    Tile** createMap(int width, int height);
    Tile** createMapMissionControl(int width, int height, int roverX, int roverY);
};