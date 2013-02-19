#pragma once

#include <OgreSceneManager.h>
#include "./Crowd/OgreRecast.h"
#include "./Crowd/OgreDetourCrowd.h"

#include "Vector3.h"

struct PathfindAgent
{
public:
  PathfindAgent(int id, OgreDetourCrowd* dtCrowd)
    : id(id),
      detour(dtCrowd),
      agent(dtCrowd->getAgent(id))
  {
  }

  void setNewTarget(Vector3 target)
  {
    detour->setMoveTarget(id, target, false);
  }

  void setChaseTarget(Vector3 target)
  {
    detour->setMoveTarget(id, target, true);
  }

  void stop()
  {
    detour->stopAgent(id);
  }

  Vector3 getPosition()
  {
    Vector3 position;
    OgreRecast::FloatAToOgreVect3(agent->npos, position);
    return position;
  }

  int getID()
  {
    return id;
  }

  bool isAtDestination(float distanceTolerance = 1.0f)
  {
    return detour->destinationReached(agent, distanceTolerance);
  }

private:
  int id;
  OgreDetourCrowd* detour;
  const dtCrowdAgent* agent;
};


class ScenePathfindManager
{
public:
  ScenePathfindManager(Ogre::SceneManager* sceneManager);
  virtual ~ScenePathfindManager(void);

  void update(double elapsedSeconds);

  void build();
  void addGeometry(Ogre::Entity* geometry);

  Vector3 getClosestNavigablePoint(Vector3 point);
  Vector3 getRandomNavigablePoint();
  Vector3 getRandomNavigablePointInCircle(Vector3 centre, double radius);

  void setDrawNavigationMesh(bool enabled);

  PathfindAgent* createAgent(Vector3 position);
  void removeAgent(PathfindAgent* agent);

  bool isReachable(Vector3 start, Vector3 end);

private:
  bool built;

  OgreRecast* recast;
  OgreDetourCrowd* detour;

  std::vector<PathfindAgent*> agents;

  std::vector<Ogre::Entity*> geometry;
  InputGeom* geom;
};

