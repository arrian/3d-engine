#include "GenerateDungeon.h"

namespace Generator
{
  Dungeon::Dungeon(void)
    : breadth(60),
      width(60)
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

  void Dungeon::construct(Architecture* architecture)
  {
    for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
    {
      (*iter)->construct(architecture);
    }

    for(std::vector<Corridor*>::iterator iter = corridors.begin(); iter < corridors.end(); ++iter)
    {
      (*iter)->construct(architecture);
    }
  }

  void Dungeon::output()
  {
    std::vector<std::vector<int>> output = std::vector<std::vector<int>>();

    //filling vector
    for(int i = 0; i < width; i++)
    {
      std::vector<int> temp = std::vector<int>();
      for(int j = 0; j < breadth; j++)
      {
        temp.push_back(0);
      }
      output.push_back(temp);
    }


    //filling output vector with rooms
    for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
    {
      for(int i = (*iter)->getLeft(); i < (*iter)->getRight(); i++)
      {
        for(int j = (*iter)->getTop(); j < (*iter)->getBottom(); j++)
        {
          output[i][j] = 1;
        }
      }
    }

    //filling output vector with corridors
    for(std::vector<Corridor*>::iterator iter = corridors.begin(); iter < corridors.end(); ++iter)
    {
      for(std::vector<Point>::iterator pIter = (*iter)->points.begin(); pIter < (*iter)->points.end(); ++pIter)
      {
        output[pIter->x][pIter->y] = 1;
      }
    }

    //outputting
    for(int i = 0; i < width; i++)
    {
      for(int j = 0; j < breadth; j++)
      {
        if(output[i][j] == 0) std::cout << "#";
        else std::cout << ".";
      }
      std::cout << std::endl;
    }
  }
}

