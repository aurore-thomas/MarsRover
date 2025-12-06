#include <random>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>

#include "planet.hpp"

using namespace std;

Planet::Planet(int width, int height, bool isMissionControl)
{
  this->width = width;
  this->height = height;

  if (isMissionControl) {
    this->map = createMapMissionControl(width, height);
  } else {
    this->map = createMap(width, height);
  }
}

int Planet::getWidth() const
{
  return width;
}
int Planet::getHeight() const
{
  return height;
}

Tile** Planet::getMap() const
{
  return map;
}


bool Planet::IsFreeTile(int x, int y) const
{
  int positionX = x % width;
  int positionY = y % height;

  if (map[positionX][positionY].type != EMPTY)
  {
    return false;
  }

  return true;
}

Tile** Planet::createMap(int width, int height)
{
  // Utiliser un générateur mieux seedé et sans biais
  static thread_local std::mt19937 rng(std::random_device{}());
  std::uniform_int_distribution<int> distX(0, width - 1);
  std::uniform_int_distribution<int> distY(0, height - 1);

  Tile** newMap = new Tile*[width];

  for (int i = 0; i < width; i++) {
    newMap[i] = new Tile[height];
    for (int j = 0; j < height; j++) {
      newMap[i][j] = {i, j, EMPTY};
    }
  }

  int nbObstacles = width*height * 3 / 10;
  
  //int randomX = rand() % width;
  //int randomY = rand() % height;  
  int randomX = distX(rng);
  int randomY = distY(rng);
  
  for (int k = 0; k < nbObstacles; k++) {
    if (newMap[randomX][randomY].type == EMPTY) 
    {
      newMap[randomX][randomY].type = OBSTACLE;
    } 
    //randomX = rand() % width;
    //randomY = rand() % height;
    randomX = distX(rng);
    randomY = distY(rng);
  }

  // Place the rover on the map
  while (newMap[randomX][randomY].type != EMPTY) {
    //randomX = rand() % width;
    //randomY = rand() % height;  
    randomX = distX(rng);
    randomY = distY(rng);
  }

  newMap[randomX][randomY].type = ROVER;

  return newMap;
}

Tile** Planet::createMapMissionControl(int width, int height) 
{
  Tile** newMap = new Tile*[width];

  for (int i = 0; i < width; i++) {
    newMap[i] = new Tile[height];
    for (int j = 0; j < height; j++) {
      newMap[i][j] = {i, j, UNKNOWN};
    }
  }

  return newMap;
}
