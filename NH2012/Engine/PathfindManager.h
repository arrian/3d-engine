#pragma once

#include <OgreSceneManager.h>
#include "./Crowd/OgreRecast.h"

class PathfindManager
{
public:
  PathfindManager(Ogre::SceneManager* sceneManager);
  virtual ~PathfindManager(void);

  OgreRecast* recast;

  void update(double elapsedSeconds);

  void build();

  void addEntity(Ogre::Entity* entity);
private:
  std::vector<Ogre::Entity*> entities;
  InputGeom* geom;
};

