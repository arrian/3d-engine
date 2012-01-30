#include "World.h"


World::World(Ogre::SceneManager* sceneManager)
{
  this->sceneManager = sceneManager;

  //sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  flags = Flag();
  dungeons = std::vector<Dungeon>();
  dungeons.push_back(Dungeon(sceneManager,"Many Monsters",DungeonType::PREDEFINED,20,1));

  if(flags.isDebug())
  {
    sceneManager->setDisplaySceneNodes(true);
    sceneManager->setShowDebugShadows(true);
    sceneManager->showBoundingBoxes(true);
  }
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
