#include "World.h"


World::World(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window)
{
  this->sceneManager = sceneManager;

  flags = new Flag();
  flags->parseIni("c:\\nh2012.ini");

  if(flags->enableShadows) sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  dungeons = std::vector<Dungeon*>();
  dungeons.push_back(new Dungeon(sceneManager, window, flags, "Entrance", DungeonType::PREDEFINED, 0, 0));

  if(flags->isDebug())
  {
    sceneManager->setDisplaySceneNodes(true);
    sceneManager->setShowDebugShadows(true);
    sceneManager->showBoundingBoxes(true);
  }
}

World::~World(void)
{
  for(std::vector<Dungeon*>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    if(*it) delete (*it);
  }

  if(flags) delete flags;
  flags = 0;
}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  for(std::vector<Dungeon*>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    (*it)->frameRenderingQueued(evt);
  }
  return true;
}

void World::injectKeyDown(const OIS::KeyEvent &arg)
{
  for(std::vector<Dungeon*>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    (*it)->injectKeyDown(arg);
  }
  
}

void World::injectKeyUp(const OIS::KeyEvent &arg)
{
  for(std::vector<Dungeon*>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    (*it)->injectKeyUp(arg);
  }
  
}

void World::injectMouseMove(const OIS::MouseEvent &arg)
{
  for(std::vector<Dungeon*>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    (*it)->injectMouseMove(arg);
  }
  
}

void World::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  for(std::vector<Dungeon*>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    (*it)->injectMouseDown(arg,id);
  }
  
}

void World::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  for(std::vector<Dungeon*>::iterator it = dungeons.begin(); it != dungeons.end(); ++it) 
  {
    (*it)->injectMouseUp(arg,id);
  }
  
}

