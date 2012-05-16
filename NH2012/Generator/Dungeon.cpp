#include "Dungeon.h"

namespace Generator
{
  Dungeon::Dungeon(void)
    : breadth(60),
      width(60)
  {
    rooms = std::vector<Room*>();
    addRooms(15);
    connectRooms();
  }

  Dungeon::Dungeon(int width, int breadth, int numberOfRooms)
    : breadth(breadth),
      width(width)
  {
    this->rooms = std::vector<Room*>();
    addRooms(numberOfRooms);
    connectRooms();
  }

  Dungeon::~Dungeon(void)
  {
  }

  void Dungeon::addRooms(int numberOfRooms)
  {
    srand(1);//time(NULL)

    int totalIterations = 0;
    int acceptableIterations = numberOfRooms * 4;
    int roomCounter = 0;
    for(; roomCounter < numberOfRooms && totalIterations < acceptableIterations; roomCounter++)
    {
      Room* room = new Room(Point(rand() % width, rand() % breadth), rand() % 5 + 2, rand() % 5 + 2);

      bool noIntersect = true;
      for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
      {
        if(room->intersects(*iter))
        {
          noIntersect = false;
          roomCounter--;
          delete room;
          break;
        }
      }

      if(noIntersect) rooms.push_back(room);

      totalIterations++;
    }

    if(numberOfRooms != roomCounter) std::cout << "Could only fit " << roomCounter << " of " << numberOfRooms << " rooms into the dungeon." << std::endl;
  }

  void Dungeon::connectRooms()
  {
    for(std::vector<Room*>::iterator roomIter = rooms.begin(); roomIter < rooms.end(); ++roomIter)
    {
      if((*roomIter)->hasEntrance()) continue;
      Room* closest = closestRoom(*roomIter);
      if(!closest) continue;
      (*roomIter)->connectRooms(closest);
    }
  }

  Room* Dungeon::closestRoom(Room* room)
  {
    Room* closest = 0;
    int shortest = INT_MAX;
    for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
    {
      if(room == *iter) continue;
      int current = (int) room->distance(*iter);
      if(current < shortest)
      {
        shortest = current;
        closest = *iter;
      }
    }

    return closest;
  }

  void Dungeon::output()
  {
    char output[100][100];

    for(int i = 0; i < width; i++) for(int j = 0; j < breadth; j++) output[i][j] = ' ';

    for(std::vector<Room*>::iterator iter = rooms.begin(); iter < rooms.end(); ++iter)
    {
      for(int i = (*iter)->topLeft.x; i <= (*iter)->topRight.x; i++) for(int j = (*iter)->topLeft.y; j <= (*iter)->bottomLeft.y; j++) output[i][j] = '#';
      for(std::vector<Entrance*>::iterator entrances = (*iter)->entrances.begin(); entrances < (*iter)->entrances.end(); ++entrances) output[(*entrances)->position.x][(*entrances)->position.y] = 'E';
    }

    //outputting
    for(int i = 0; i < width; i++)
    {
      for(int j = 0; j < breadth; j++) std::cout << output[i][j];
      std::cout << std::endl;
    }
  }
}

