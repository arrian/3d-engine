#pragma once

#include <vector>
#include "Point.h"
#include "Room.h"
//#include "Architecture.h"


namespace Generator
{
  class Corridor
  {
  public:
    Corridor(Room* start);
    ~Corridor(void);

    /**
     * Checks if this corridor contains the given room. Note: only checks address.
     */
    bool contains(Room* room);

    /**
     * Links this corridor to the given room.
     */
    void link(Room* room);

    /**
     * Links the two corridors changing this and discarding other.
     */
    void link(Corridor* other);

    /**
     * Checks if this and the given corridor are linked to the same room
     */ 
    bool isConnected(Corridor* other);

    /**
     * Constructs the mesh architecture for this corridor.
     */
    //void construct(Architecture* architecture);

    std::vector<Room*> rooms;
  };
}
