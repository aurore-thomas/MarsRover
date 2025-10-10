#pragma once
#include <list>

using namespace std;

enum ObjectType {EMPTY, ROVER, OBSTACLE}; 

struct Case {
    int x;
    int y;
    ObjectType type;
    bool discovered;
};

class Planet {
  private:
    int width;
    int height;
    Case** map;

  public:
    Planet(int width, int height);

    int getWidth() const;
    void setWidth(int width);
    int getHeight() const;
    void setHeight(int height);

    Case** getMap() const;
    void setMap(Case** map);

    bool CaseIsFree(int x, int y) const;
    Case** createMap(int width, int height);

};