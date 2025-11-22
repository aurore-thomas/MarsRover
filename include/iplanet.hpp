#pragma once

enum ObjectType {EMPTY, ROVER, OBSTACLE, UNKNOWN}; 
enum Orientation {NORTH, EAST, SOUTH, WEST};
enum Command {MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT};

class IPlanet
{
  public:
    virtual bool IsFreeTile(int x, int y) const = 0;
};