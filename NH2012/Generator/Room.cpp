#include "Room.h"

namespace Generator
{
  Room::Room(Point position, int width, int breadth)
    : entrances(std::vector<Point>())
  {

  }

  Room::~Room(void)
  {
  }

  void Room::addEntrance(Point point)
  {
    entrances.push_back(point);
  }

  bool Room::isEntrance(Point point)
  {
    for(std::vector<Point>::iterator iter = entrances.begin(); iter < entrances.end(); ++iter)
    {
      if((*iter) == point) return true;
    }
    return false;
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

