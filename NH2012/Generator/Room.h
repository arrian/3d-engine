#pragma once

#include <vector>
#include "Point.h"
#include "Entrance.h"
//#include "Architecture.h"

namespace Generator
{
  class Room
  {
  public:
    Room(Point position, int width, int breadth);
    ~Room(void);

    Point topLeft;
    Point topRight;
    Point bottomLeft;
    Point bottomRight;
    std::vector<Point*> corners;

    std::vector<Entrance*> entrances;
    std::vector<Point> potentialEntrances;

    Point getCentre();
    bool intersects(Room* other);
    double distance(Room* other);

    void addEntrance(Entrance* entrance);
    bool hasEntrance();
    bool isEntrance(Point point);

    bool isInside(Point point);

    /*! Finds the shortest connection between this and the other room, creates entrances and connects them.*/
    void connectRooms(Room* other);

    //void construct(Architecture* architecture);
  };
}
