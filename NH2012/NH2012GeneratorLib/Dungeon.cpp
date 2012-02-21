#include "Dungeon.h"

namespace Generator
{
  Dungeon::Dungeon(void)
  {
    rooms = std::vector<Room*>();
    addRooms(300);

    corridors = std::vector<Corridor*>();
    addCorridors();
  }

  Dungeon::~Dungeon(void)
  {
  }

  void Dungeon::addRooms(int maxRooms)
  {
    srand(1);//time(NULL)

    for(int i = 0; i < maxRooms; i++)
    {
      Room* room = new Room(Point(rand() % 50, rand() % 50), rand() % 6 + 3, rand() % 6 + 3);

      bool noIntersect = true;
      for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
      {
        if(room->roomIntersects(*iter))
        {
          noIntersect = false;
          delete room;
          break;
        }
      }

      if(noIntersect) rooms.push_back(room);
    }
  }

  void Dungeon::addCorridors()
  {
    for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
    {
      Room* nearest = getNearestRoom(*iter);

      Point start = nearest->getEntrance(*iter);
      Point end = (*iter)->getEntrance(nearest);

      corridors.push_back(new Corridor(start,end));
    }
  }

  Room* Dungeon::getNearestRoom(Room* room)
  {
    Room* nearest = room;
    double distance = 100000;

    for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
    {
      double test = room->getCentre().distance((*iter)->getCentre());
      if(test < distance && test != 0)
      {
        nearest = *iter;
        distance = test;
      }
    }

    return nearest;
  }

  void Dungeon::output()
  {
    for(int i = 0; i < 60; i++)
    {
      for(int j = 0; j < 60; j++)
      {
        bool isRoom = false;
        for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
        {
          if((*iter)->pointInside(Point(i,j)))
          {
            isRoom = true;
            break;
          }
        }
        for(std::vector<Corridor*>::iterator iter = corridors.begin(); iter < corridors.end(); ++iter)
        {
          if((*iter)->pointInside(Point(i,j)))
          {
            isRoom = true;
            break;
          }
        }
        if(isRoom) std::cout << ".";
        else std::cout << "#";
      }
      std::cout << std::endl;
    }
  }
}

