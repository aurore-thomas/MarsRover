#pragma once
#include <vector.h>

class Obstacle  {
  public:
    Obstacle(Vector<int> pos, bool status);

    bool isDetected();
    void setDetected(bool status);

  private:
    Vector<int> position;
    bool detected;
};