#pragma once

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Point.h"
#include "Room.h"

//#include "Architecture.h"

namespace Generator
{
  class Dungeon
  {
  public:
    Dungeon(void);
    Dungeon(int width, int breadth, int numberOfRooms);
    ~Dungeon(void);

    std::vector<Room*> rooms;

    void addRooms(int numberOfRooms);
    void connectRooms();

    Room* closestRoom(Room* room);

    void output();

    int width;
    int breadth;
  };
}

