#pragma once

#include <vector>
#include "Point.h"

namespace Generator
{
  class Corridor
  {
  public:
    Corridor(Point start, Point end);
    ~Corridor(void);

    std::vector<Point> points;

    bool pointInside(Point point);
  };
}
