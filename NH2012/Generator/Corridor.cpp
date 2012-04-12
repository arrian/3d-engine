#include "Corridor.h"

namespace Generator
{
  Corridor::Corridor(Room* start)
  {
    rooms = std::vector<Room*>();
    link(start);
  }

  Corridor::~Corridor(void)
  {
  }

  bool Corridor::contains(Room* room)
  {
    for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); iter++)
    {
      if(room == *iter) return true;
    }
    return false;
  }

  void Corridor::link(Room* room)
  {
    rooms.push_back(room);
  }

  void Corridor::link(Corridor* other)
  {
    for(std::vector<Room*>::iterator iter = other->rooms.begin(); iter < other->rooms.end(); iter++)
    {
      if(!contains(*iter)) rooms.push_back(*iter);
    }
  }

  bool Corridor::isConnected(Corridor* other)
  {
    for(std::vector<Room*>::iterator iter = other->rooms.begin(); iter < other->rooms.end(); iter++)
    {
      if(contains(*iter)) return true;
    }
    return false;
  }

  /*
  void Corridor::construct(Architecture* architecture)
  {
    for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
    {
      architecture->add("hall.mesh", Ogre::Vector3(100*(*iter)->getCentre().x,0,100*(*iter)->getCentre().y));
    }
  }
  */
}

