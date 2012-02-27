#pragma once

#include <vector>
#include "GeneratePoint.h"
#include "Architecture.h"


namespace Generator
{
  class Corridor
  {
  public:
    Corridor(Point start, Point end);
    ~Corridor(void);

    std::vector<Point> points;

    bool pointInside(Point point);

    void construct(Architecture* architecture);
  };
}
