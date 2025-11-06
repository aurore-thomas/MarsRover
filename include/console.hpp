#pragma once

#include "planet.hpp"
#include "communication.hpp"

using namespace std;

class Console
{
  public:
    Console();
    void displayMap(int width, int height, Tile** map, Orientation orientation) const;
};
 