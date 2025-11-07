#pragma once
#include <list>

using namespace std;

enum ObjectType {EMPTY, ROVER, OBSTACLE, UNKNOWN}; 
enum Orientation {NORTH, EAST, SOUTH, WEST};
enum Command {MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT};

struct Tile {
    int x;
    int y;
    ObjectType type;
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
    bool hasUnknownTiles() const;
    Tile** createMap(int width, int height);
    Tile** createMapMissionControl(int width, int height);
    void updateMapWithDiscoveredTiles(const int x, const int y, const ObjectType type);
};