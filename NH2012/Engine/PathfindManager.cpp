#include "PathfindManager.h"


PathfindManager::PathfindManager(Ogre::SceneManager* sceneManager)
  : OgreRecast(sceneManager, OgreRecastConfigParams()),
    entities(),
    geom(NULL)
{
}

//-------------------------------------------------------------------------------------
PathfindManager::~PathfindManager(void)
{
  if(geom) delete geom;
}

//-------------------------------------------------------------------------------------
void PathfindManager::update(double elapsedSeconds)
{
  OgreRecast::update();
}

//-------------------------------------------------------------------------------------
void PathfindManager::addEntity(Ogre::Entity* entity)
{
  entities.push_back(entity);
}

//-------------------------------------------------------------------------------------
void PathfindManager::build()
{
  std::cout << "Building navigation... ";
  geom = new InputGeom(entities);
  NavMeshBuild(geom);
  std::cout << "done." << std::endl;
}