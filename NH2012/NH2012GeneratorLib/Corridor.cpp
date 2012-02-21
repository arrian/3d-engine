#include "Corridor.h"

namespace Generator
{
  Corridor::Corridor(Point start, Point end)
  {
    points.push_back(start);
    points.push_back(end);

    int xIter = start.x;
    while(xIter != end.x)
    {
      if(start.x < end.x) xIter++;
      else xIter--;

      points.push_back(Point(xIter,start.y));
    }

    int yIter = start.y;
    while(yIter != end.y)
    {
      if(start.y < end.y) yIter++;
      else yIter--;

      points.push_back(Point(xIter,yIter));
    }
  }


  Corridor::~Corridor(void)
  {
  }

  bool Corridor::pointInside(Point point)
  {
    for(std::vector<Point>::iterator iter = points.begin(); iter < points.end(); ++iter)
    {
      if(iter->x == point.x && iter->y == point.y) return true;
    }
    return false;
  }
}

