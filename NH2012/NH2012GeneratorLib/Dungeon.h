#pragma once

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Point.h"
#include "Room.h"
#include "Corridor.h"

namespace Generator
{
  class Dungeon
  {
  public:
    Dungeon(void);
    ~Dungeon(void);

    std::vector<Room*> rooms;
    std::vector<Corridor*> corridors;

    void addRooms(int maxRooms);
    void addCorridors();

    Room* getNearestRoom(Room* room);

    void output();
  };
}

