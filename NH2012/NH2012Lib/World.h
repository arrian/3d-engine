#pragma once

#include <vector>

#include <OgreFrameListener.h>

#include "Monster.h"
#include "Item.h"
#include "Dungeon.h"

struct WorldData
{
  int timestamp;

  std::vector<Monster> monsters;
  std::vector<Item> items;
  Dungeon dungeon;
};

class World
{
public:
  World(void);
  ~World(void);

  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations

  WorldData getWorldState();
private:
  WorldData data;
};

