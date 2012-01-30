#include "World.h"


World::World(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window) : player(sceneManager, window, Ogre::Vector3(0,0,0))
{
  this->sceneManager = sceneManager;

  //sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  flags = Flag();
  dungeons = std::vector<Dungeon>();
  dungeons.push_back(Dungeon(sceneManager,"Entrance",DungeonType::PREDEFINED,20,1));
  player.setDungeon(&dungeons.front());

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
  player.frameRenderingQueued(evt);
  for(std::vector<Dungeon>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    it->frameRenderingQueued(evt);
  }
  return true;
}

void World::injectKeyDown(const OIS::KeyEvent &arg)
{
  player.injectKeyDown(arg);
}

void World::injectKeyUp(const OIS::KeyEvent &arg)
{
  player.injectKeyUp(arg);
}

void World::injectMouseMove(const OIS::MouseEvent &arg)
{
  player.injectMouseMove(arg);
}

void World::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player.injectMouseDown(arg,id);
}

void World::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player.injectMouseUp(arg,id);
}
