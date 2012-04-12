#pragma once

#include <vector>
#include "Point.h"
//#include "Architecture.h"

namespace Generator
{

  class Room
  {
  public:
    Room(Point position, int width, int breadth);
    ~Room(void);

    std::vector<Point> corners;
    std::vector<Point> entrances;

    void addEntrance(Point point);
    bool isEntrance(Point point);

    //void construct(Architecture* architecture);
  };
}
