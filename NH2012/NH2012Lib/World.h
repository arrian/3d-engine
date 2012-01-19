#pragma once

#include <vector>

#include <OgreFrameListener.h>

#include "Monster.h"
#include "Inventory.h"
#include "Dungeon.h"
#include "Flag.h"

class Monster;//forward declaring for circular dependency

class World
{
public:
  World(void);
  ~World(void);

  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations
private:
  int timestamp;

  std::vector<Monster> monsters;
  Inventory items;
  Dungeon dungeon;
  Flag flags;
};

