#include <random>

#include "planet.hpp"

using namespace std;

Planet::Planet(int width, int height) 
{
  setWidth(width);
  setHeight(height);
  setMap(createMap(width, height));
}

int Planet::getWidth() const
{
  return width;
}
void Planet::setWidth(int width)
{
  this->width = width;
}

int Planet::getHeight() const
{
  return height;
}
void Planet::setHeight(int height)
{
  this->height = height;
}

Tile** Planet::getMap() const
{
  return map;
}
void Planet::setMap(Tile** map)
{
  this->map = map;
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
  Tile** newMap = new Tile*[width];

  for (int i = 0; i < width; i++) {
    newMap[i] = new Tile[height];
    for (int j = 0; j < height; j++) {
      newMap[i][j] = {i, j, EMPTY, false};
    }
  }

  int nbObstacles = width*height * 3 / 10;

  int randomX = rand() % width;
  int randomY = rand() % height;
  
  for (int k = 0; k < nbObstacles; k++) {
    if (newMap[randomX][randomY].type == EMPTY) 
    {
      newMap[randomX][randomY].type = OBSTACLE;
    } 

    randomX = rand() % width;
    randomY = rand() % height;
  }

  // Place the rover on the map
  while (newMap[randomX][randomY].type != EMPTY) {
    randomX = rand() % width;
    randomY = rand() % height;
  }

  newMap[randomX][randomY].type = ROVER;

  return newMap;
}
