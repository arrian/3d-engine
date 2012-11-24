#include "PathfindManager.h"


PathfindManager::PathfindManager(Ogre::SceneManager* sceneManager)
  : recast(NULL),
    entities(),
    geom(NULL)
{
  OgreRecastConfigParams params;
  //set up parameters here

  recast = new OgreRecast(sceneManager, params);
}

PathfindManager::~PathfindManager(void)
{
  if(recast) delete recast;
  if(geom) delete geom;
}

void PathfindManager::update(double elapsedSeconds)
{
  recast->update();
}

void PathfindManager::addEntity(Ogre::Entity* entity)
{
  entities.push_back(entity);
}

void PathfindManager::build()
{
  std::cout << "building navigation" << std::endl;
  geom = new InputGeom(entities);
  recast->NavMeshBuild(geom);
  recast->drawNavMesh();
}