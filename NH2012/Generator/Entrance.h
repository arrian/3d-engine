#pragma once
#include "Point.h"

namespace Generator
{
  class Entrance
  {
  public:
    Entrance(Point position);
    virtual ~Entrance(void);

    Point position;    

    Entrance* other;
  };
}
