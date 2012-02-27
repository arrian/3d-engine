#include "GenerateRoom.h"

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

  void Room::construct(Architecture* architecture)
  {
    for(int i = getLeft(); i <= getRight(); i++)
    {
      for(int j = getTop(); j <= getBottom(); j++)
      {
        Point current(i,j);

        Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
        //rotation.FromAngleAxis(Ogre::Degree(0),Ogre::Vector3::UNIT_Y);

        //corners
        if(getTopLeft() == current) 
        {
          rotation.FromAngleAxis(Ogre::Degree(0),Ogre::Vector3::UNIT_Y);
          architecture->add("room_corner.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
        }
        else if(getTopRight() == current) 
        {
          rotation.FromAngleAxis(Ogre::Degree(270),Ogre::Vector3::UNIT_Y);
          architecture->add("room_corner.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
        }
        else if(getBottomLeft() == current) 
        {
          rotation.FromAngleAxis(Ogre::Degree(90),Ogre::Vector3::UNIT_Y);
          architecture->add("room_corner.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
        }
        else if(getBottomRight() == current) 
        {
          rotation.FromAngleAxis(Ogre::Degree(180),Ogre::Vector3::UNIT_Y);
          architecture->add("room_corner.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
        }

        //sides
        else if(getTop() == current.y) 
        {
          rotation.FromAngleAxis(Ogre::Degree(90),Ogre::Vector3::UNIT_Y);
          architecture->add("room_edge.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
        }
        else if(getBottom() == current.y) 
        {
          rotation.FromAngleAxis(Ogre::Degree(270),Ogre::Vector3::UNIT_Y);
          architecture->add("room_edge.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
        }
        else if(getLeft() == current.x) 
        {
          rotation.FromAngleAxis(Ogre::Degree(180),Ogre::Vector3::UNIT_Y);
          architecture->add("room_edge.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
        }
        else if(getRight() == current.x) 
        {
          rotation.FromAngleAxis(Ogre::Degree(0),Ogre::Vector3::UNIT_Y);
          architecture->add("room_edge.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
        }

        //centre
        else architecture->add("room_centre.mesh", Ogre::Vector3(100*i,0,100*j), rotation);
      }
    }
  }
}
