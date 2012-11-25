#pragma once

#include <OgreSceneManager.h>
#include "./Crowd/OgreRecast.h"

class PathfindManager : public OgreRecast
{
public:
  PathfindManager(Ogre::SceneManager* sceneManager);
  virtual ~PathfindManager(void);

  

  void update(double elapsedSeconds);

  void build();

  void addEntity(Ogre::Entity* entity);
private:
  //OgreRecast* recast;

  std::vector<Ogre::Entity*> entities;
  InputGeom* geom;
};

