#include "PathfindManager.h"

#include "NHException.h"

PathfindManager::PathfindManager(Ogre::SceneManager* sceneManager)
  : recast(NULL),
    geometry(),
    geom(NULL)
{
  OgreRecastConfigParams params = OgreRecastConfigParams();
  //set required parameters here
  recast = new OgreRecast(sceneManager, params);
}

//-------------------------------------------------------------------------------------
PathfindManager::~PathfindManager(void)
{
  if(geom) delete geom;
  if(recast) delete recast;
}

//-------------------------------------------------------------------------------------
void PathfindManager::update(double elapsedSeconds)
{
  recast->update();
}

//-------------------------------------------------------------------------------------
void PathfindManager::addGeometry(Ogre::Entity* geometry)
{
  this->geometry.push_back(geometry);
}

//-------------------------------------------------------------------------------------
void PathfindManager::build()
{
  std::cout << "Building navigation... ";
  geom = new InputGeom(geometry);
  recast->NavMeshBuild(geom);
  std::cout << "done." << std::endl;
}

//-------------------------------------------------------------------------------------
std::vector<Ogre::Vector3> PathfindManager::getPath(Ogre::Vector3 start, Ogre::Vector3 end)
{
  int code = recast->FindPath(start, end, 0, 0);
  switch(code)
  {
    case 0: break;
    case -1: std::cout << "Monster Pathfind: No polygon near start point." << std::endl; break;
    case -2: std::cout << "Monster Pathfind: No polygon near end point." << std::endl; break;
    case -3: std::cout << "Monster Pathfind: Could not create a path." << std::endl; break;
    case -4: std::cout << "Monster Pathfind: Could not find a path." << std::endl; break;
    case -5: std::cout << "Monster Pathfind: Could not create a straight path." << std::endl; break;
    case -6: std::cout << "Monster Pathfind: Could not find a straight path." << std::endl; break;
    default: break;
  }
  return recast->getPath(0);
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 PathfindManager::getClosestNavigablePoint(Ogre::Vector3 point)
{
  Ogre::Vector3 result;
  if(recast->findNearestPointOnNavmesh(point, result)) return result;

  throw NHException("failed to find the closest navigable point");
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 PathfindManager::getRandomNavigablePoint()
{
  return recast->getRandomNavMeshPoint();
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 PathfindManager::getRandomNavigablePointInCircle(Ogre::Vector3 centre, double radius)
{
  return recast->getRandomNavMeshPointInCircle(centre, radius);
}

//-------------------------------------------------------------------------------------
void PathfindManager::setDrawNavigationMesh(bool enabled)
{
  if(enabled) recast->drawNavMesh();
  else throw NHException("removing a navigation mesh is not implemented");
  
}

