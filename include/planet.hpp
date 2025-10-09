#pragma once
#include <list>

#include "obstacle.hpp"

using namespace std;

class Planet {
  private:
    int width;
    int height;
    list<Obstacle> obstacles;
};