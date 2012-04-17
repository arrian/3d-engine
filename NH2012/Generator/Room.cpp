#include "Room.h"

namespace Generator
{
  Room::Room(Point position, int width, int breadth)
    : entrances(std::vector<Entrance*>()),
      potentialEntrances(std::vector<Point>())
  {
    topLeft = Point(position);
    topRight = Point(position + Point(width, 0));
    bottomLeft = Point(position + Point(0, breadth));
    bottomRight = Point(position + Point(width, breadth));

    corners = std::vector<Point*>();
    corners.push_back(&topLeft);
    corners.push_back(&bottomLeft);
    corners.push_back(&bottomRight);
    corners.push_back(&topRight);

    //determining potential entrances
    for(Point iter = topLeft + Point(0, -1); iter.x <= topRight.x; iter.x++) potentialEntrances.push_back(iter);
    for(Point iter = topRight + Point(1, 0); iter.y <= bottomRight.y; iter.y++) potentialEntrances.push_back(iter);
    for(Point iter = bottomRight + Point(0, 1); iter.x >= bottomLeft.x; iter.x--) potentialEntrances.push_back(iter);
    for(Point iter = bottomLeft + Point(-1, 0); iter.y >= topLeft.y; iter.y--) potentialEntrances.push_back(iter);
  }

  Room::~Room(void)
  {
  }

  Point Room::getCentre()
  {
    return Point((topRight.x + topLeft.x) / 2, (bottomLeft.y + topLeft.y) / 2);
  }

  bool Room::intersects(Room* other)
  {
    //note: leaving buffer space of 1 square around this room for entrances
    return (topLeft.x - 1 < other->bottomRight.x && 
            bottomRight.x + 1 >= other->topLeft.x &&
            topLeft.y - 1 < other->bottomRight.y && 
            bottomRight.y + 1 >= other->topLeft.y);
  }

  double Room::distance(Room* other)
  {
    return getCentre().distance(other->getCentre());
  }

  void Room::addEntrance(Entrance* entrance)
  {
    entrances.push_back(entrance);
  }

  bool Room::hasEntrance()
  {
    return entrances.size() > 0;
  }

  bool Room::isEntrance(Point point)
  {
    for(std::vector<Entrance*>::iterator iter = entrances.begin(); iter < entrances.end(); ++iter)
    {
      if((*iter)->position == point) return true;
    }
    return false;
  }

  bool Room::isInside(Point point)
  {
    return (point.x >= topLeft.x && point.x <= topRight.x && point.y >= topLeft.y && point.y <= bottomLeft.y);
  }

  void Room::connectRooms(Room* other)
  {
    Point closest;
    Point closestOther;
    int shortest = INT_MAX;
    int current = 0;

    //finding closest potential entrances
    for(std::vector<Point>::iterator iter = potentialEntrances.begin(); iter < potentialEntrances.end(); ++iter)
    {
      for(std::vector<Point>::iterator iterOther = other->potentialEntrances.begin(); iterOther < other->potentialEntrances.end(); ++iterOther)
      {
        current = iter->distanceTaxicab(*iterOther);
        if(current < shortest)
        {
          closest = *iter;
          closestOther = *iterOther;
          shortest = current;
        }
      }
    }

    //ensuring suitable connection was found
    if(shortest != INT_MAX)
    {
      //creating entrances
      Entrance* entrance = new Entrance(closest);
      Entrance* entranceOther = new Entrance(closestOther);

      //connecting entrances
      entrance->other = entranceOther;
      entranceOther->other = entrance;

      //adding entrances to rooms
      addEntrance(entrance);
      other->addEntrance(entranceOther);
    }
  }


  /*
  void Room::construct(Architecture* architecture)
  {
    for(int i = getLeft(); i <= getRight(); i++)
    {
      for(int j = getTop(); j <= getBottom(); j++)
      {
        Point current(i,j);

        Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
        Ogre::String mesh = "";
        
        //corners
        if(getTopLeft() == current) 
        {
          rotation.FromAngleAxis(Ogre::Degree(0),Ogre::Vector3::UNIT_Y);
          mesh = "room_corner.mesh";
        }
        else if(getTopRight() == current) 
        {
          rotation.FromAngleAxis(Ogre::Degree(270),Ogre::Vector3::UNIT_Y);
          mesh = "room_corner.mesh";
        }
        else if(getBottomLeft() == current) 
        {
          rotation.FromAngleAxis(Ogre::Degree(90),Ogre::Vector3::UNIT_Y);
          mesh = "room_corner.mesh";
        }
        else if(getBottomRight() == current) 
        {
          rotation.FromAngleAxis(Ogre::Degree(180),Ogre::Vector3::UNIT_Y);
          mesh = "room_corner.mesh";
        }

        //sides
        else if(getTop() == current.y) 
        {
          if(isEntrance(current))
          {
            mesh = "room_edge_entrance.mesh";
          }
          else
          {
            rotation.FromAngleAxis(Ogre::Degree(90),Ogre::Vector3::UNIT_Y);
            mesh = "room_edge.mesh";
          }
        }
        else if(getBottom() == current.y) 
        {
          if(isEntrance(current))
          {
            mesh = "room_edge_entrance.mesh";
          }
          else
          {
            rotation.FromAngleAxis(Ogre::Degree(270),Ogre::Vector3::UNIT_Y);
            mesh = "room_edge.mesh";
          }
        }
        else if(getLeft() == current.x) 
        {
          if(isEntrance(current))
          {
            mesh = "room_edge_entrance.mesh";
          }
          else
          {
            rotation.FromAngleAxis(Ogre::Degree(180),Ogre::Vector3::UNIT_Y);
            mesh = "room_edge.mesh";
          }
        }
        else if(getRight() == current.x) 
        {
          if(isEntrance(current))
          {
            mesh = "room_edge_entrance.mesh";
          }
          else
          {
            rotation.FromAngleAxis(Ogre::Degree(0),Ogre::Vector3::UNIT_Y);
            mesh = "room_edge.mesh";
          }
        }

        //centre
        else 
        {
          mesh = "room_centre.mesh";
        }

        architecture->add(mesh, Ogre::Vector3(100*i,0,100*j), rotation);
      }
    }
  }
  */
}

