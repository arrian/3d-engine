#include "World.h"


World::World(Ogre::SceneManager* sceneManager)
{
  this->sceneManager = sceneManager;
  flags = Flag();
  dungeons = std::vector<Dungeon>();
  dungeons.push_back(Dungeon(sceneManager));
}

World::~World(void)
{
}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  for(std::vector<Dungeon>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    it->frameRenderingQueued(evt);
  }
  return true;
}
