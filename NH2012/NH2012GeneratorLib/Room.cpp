#include "Room.h"

namespace Generator
{
  Room::Room(Point position, int width, int breadth)
  {
    this->position = position;
    this->width = width;
    this->breadth = breadth;
  }

  Room::~Room(void)
  {
  }

  void Room::reset(Point position, int width, int breadth)
  {
    this->position = position;
    this->width = width;
    this->breadth = breadth;
  }

  Point Room::getEntrance(Room* room)
  {
    double top = room->getTopLeft().distance(getTopLeft(),getTopRight());
    double right = room->getTopLeft().distance(getTopRight(),getBottomRight());
    double bottom = room->getTopLeft().distance(getBottomRight(),getBottomLeft());
    double left = room->getTopLeft().distance(getBottomLeft(),getTopLeft());

    double min = top;
    if(min > right) min = right;
    if(min > bottom) min = bottom;
    if(min > left) min = left;

    if(min == top) return getTopLeft();//top
    if(min == right) return getTopRight();//right
    if(min == bottom) return getBottomRight();//bottom
    return getBottomLeft();//left
  }

  Point Room::getCentre()
  {
    return Point(position.x + width / 2, position.y + breadth / 2);
  }

  Point Room::getTopLeft()
  {
    return position;
  }

  Point Room::getTopRight()
  {
    return Point(getRight(),getTop());
  }

  Point Room::getBottomLeft()
  {
    return Point(getLeft(),getBottom());
  }

  Point Room::getBottomRight()
  {
    return Point(getRight(),getBottom());
  }

  int Room::getLeft()
  {
    return position.x;
  }

  int Room::getRight()
  {
    return position.x + width;
  }

  int Room::getTop()
  {
    return position.y;
  }

  int Room::getBottom()
  {
    return position.y + breadth;
  }

  bool Room::roomIntersects(Room* room)
  {
    return !(room->getLeft() - 1 > getRight() + 1 || 
             room->getRight() + 1 < getLeft() - 1 || 
             room->getTop() - 1 > getBottom() + 1 ||
             room->getBottom() + 1 < getTop() - 1);
  }

  bool Room::pointInside(Point point)
  {
    return (point.x <= getRight() && point.x >= getLeft()) && 
           (point.y <= getBottom() && point.y >= getTop());
  }
}
