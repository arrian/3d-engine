#pragma once

#include <vector>
#include "Point.h"

namespace Generator
{
  class Room
  {
  public:
    Room(Point position, int width, int breadth);
    ~Room(void);

    Point position;
    int width;
    int breadth;

    void reset(Point position, int width, int breadth);

    /*! Gets an entrance to the room closest to the given room and returns its position.*/
    Point getEntrance(Room* room);

    Point getCentre();

    Point getTopLeft();
    Point getTopRight();
    Point getBottomLeft();
    Point getBottomRight();

    int getLeft();
    int getRight();
    int getTop();
    int getBottom();

    bool roomIntersects(Room* room);
    bool pointInside(Point point);
  };
}
