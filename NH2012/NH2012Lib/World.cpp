#include "World.h"


World::World(void)
{
  flags = Flag();

  monsters = std::vector<Monster>();
  items = Inventory();
  dungeon = Dungeon("Test Dungeon", DungeonType::DUNGEON);
}


World::~World(void)
{
}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  return true;
}
