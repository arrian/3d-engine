#pragma once

#include "Scene.h"
#include "./Crowd/OgreRecast.h"

class PathfindManager
{
public:
  PathfindManager(Scene* scene);
  virtual ~PathfindManager(void);

  OgreRecast* recast;

  void update(double elapsedSeconds);

  void build();

  void addEntity(Ogre::Entity* entity);
private:
  std::vector<Ogre::Entity*> entities;
  InputGeom* geom;
};

