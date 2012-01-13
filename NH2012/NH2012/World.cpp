#include "World.h"


World::World(void)
{
}


World::~World(void)
{
}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  return true;
}

WorldData World::getWorldState()
{
  return data;
}