#include "ScenePathfindManager.h"

#include "NHException.h"

ScenePathfindManager::ScenePathfindManager(Ogre::SceneManager* sceneManager)
  : recast(NULL),
    detour(NULL),
    geometry(),
    geom(NULL),
    agents(),
    built(false)
{
  OgreRecastConfigParams params = OgreRecastConfigParams();
  //set required parameters here
  recast = new OgreRecast(sceneManager, params);
}

//-------------------------------------------------------------------------------------
ScenePathfindManager::~ScenePathfindManager(void)
{
  delete geom;
  delete recast;

  for(std::vector<PathfindAgent*>::iterator iter = agents.begin(); iter != agents.end(); ++iter)
  {
    delete *iter;
  }
}

//-------------------------------------------------------------------------------------
void ScenePathfindManager::update(double elapsedSeconds)
{
  if(!built) return;
  recast->update();
  detour->updateTick((float) elapsedSeconds);
}

//-------------------------------------------------------------------------------------
void ScenePathfindManager::addGeometry(Ogre::Entity* geometry)
{
  this->geometry.push_back(geometry);
}

//-------------------------------------------------------------------------------------
void ScenePathfindManager::build()
{
  std::cout << "Building navigation... ";
  geom = new InputGeom(geometry);
  recast->NavMeshBuild(geom);
  built = true;
  detour = new OgreDetourCrowd(recast);
  if(!detour) throw NHException("could not create detour crowd in pathfind manager");
  std::cout << "done." << std::endl;

    /*
  mDetourTileCache = new OgreDetourTileCache(mRecast);
  if(mDetourTileCache->TileCacheBuild(mNavmeshEnts)) {
  mDetourTileCache->drawNavMesh();
  } else {
  Ogre::LogManager::getSingletonPtr()->logMessage("ERROR: could not generate useable navmesh from mesh using detourTileCache.");
  return;
  }

  if(EXTRACT_WALKABLE_AREAS) {
  // Mark walkable area (where agents will be spawned)
  OgreRecastNavmeshPruner *navMeshPruner = mRecast->getNavmeshPruner();
  // Start tracing at the origin position on the navmesh, and include all areas that are reachable from there (there is no box in the center)
  navMeshPruner->floodNavmesh(Vector3::ZERO);
  navMeshPruner->pruneSelected();
  }
  
  */
}

//-------------------------------------------------------------------------------------
Vector3 ScenePathfindManager::getClosestNavigablePoint(Vector3 point)
{
  Vector3 result;
  if(recast->findNearestPointOnNavmesh(point, result)) return result;

  throw NHException("failed to find the closest navigable point");
}

//-------------------------------------------------------------------------------------
Vector3 ScenePathfindManager::getRandomNavigablePoint()
{
  return recast->getRandomNavMeshPoint();
}

//-------------------------------------------------------------------------------------
Vector3 ScenePathfindManager::getRandomNavigablePointInCircle(Vector3 centre, double radius)
{
  return recast->getRandomNavMeshPointInCircle(centre, radius);
}

//-------------------------------------------------------------------------------------
void ScenePathfindManager::setDrawNavigationMesh(bool enabled)
{
  if(enabled) recast->drawNavMesh();
  else throw NHException("removing a navigation mesh is not implemented");
}

//-------------------------------------------------------------------------------------
PathfindAgent* ScenePathfindManager::createAgent(Vector3 position)
{
  int id = detour->addAgent(position);
  if(id < 0) throw NHException("could not create an agent because the maximum number of agents have been created");
  PathfindAgent* agent = new PathfindAgent(id, detour);
  agents.push_back(agent);
  return agent;
}

//-------------------------------------------------------------------------------------
void ScenePathfindManager::removeAgent(PathfindAgent* agent)
{
  if(!agent) return;
  detour->removeAgent(agent->getID());
  agents.erase(std::remove(agents.begin(), agents.end(), agent), agents.end());
  delete agent;
}

//-------------------------------------------------------------------------------------
bool ScenePathfindManager::isReachable(Vector3 start, Vector3 end)
{
  if(recast->FindPath(start, end, 0, 0) == 0) return true;
  return false;
}
