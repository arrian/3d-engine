#pragma once

#include <vector>

#include <OgreString.h>
#include <OgreVector3.h>

#include "PxPhysicsAPI.h"

class Scene;

class Architecture
{
public:
  Architecture(Scene* scene);

  ~Architecture(void);

  /*! Adds a static mesh to the current dungeon architecture.*/
  void add(Ogre::String meshName, Ogre::Vector3 position = Ogre::Vector3(0,0,0), Ogre::Quaternion quaternion = Ogre::Quaternion::IDENTITY);

  void build();
private:
  Scene* scene;
  
  /*! Current number of physics items.*/
  int instanceNumber;

  std::vector<Ogre::SceneNode*> nodes;
  std::map<Ogre::String, Ogre::Entity*> entities;
  std::vector<physx::PxTriangleMesh*> bodies;

  Ogre::StaticGeometry* geometry;

  void addStaticTrimesh(Ogre::String meshName, Ogre::Real restitution, const Ogre::Real friction, Ogre::Vector3 position, Ogre::Quaternion quaternion);
};

