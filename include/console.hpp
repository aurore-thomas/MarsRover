#pragma once

#include "planet.hpp"
#include "communication.hpp"

using namespace std;

class Console
{
  public:
    Console();
    void displayMap(int width, int height, Case** map, Orientation orientation) const;
};
 