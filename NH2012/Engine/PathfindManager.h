#pragma once

#include <OgreSceneManager.h>
#include "./Crowd/OgreRecast.h"

class PathfindManager
{
public:
  PathfindManager(Ogre::SceneManager* sceneManager);
  virtual ~PathfindManager(void);

  void update(double elapsedSeconds);

  void build();
  void addGeometry(Ogre::Entity* geometry);

  std::vector<Ogre::Vector3> getPath(Ogre::Vector3 start, Ogre::Vector3 end);
  Ogre::Vector3 getClosestNavigablePoint(Ogre::Vector3 point);
  Ogre::Vector3 getRandomNavigablePoint();
  Ogre::Vector3 getRandomNavigablePointInCircle(Ogre::Vector3 centre, double radius);

  void setDrawNavigationMesh(bool enabled);
private:
  OgreRecast* recast;

  std::vector<Ogre::Entity*> geometry;
  InputGeom* geom;
};

