#pragma once

#include <vector>

#include <OgreString.h>
#include "Vector3.h"
#include <OgreSceneNode.h>

#include "Identifiable.h"
#include "DataManager.h"

#include "PxPhysicsAPI.h"

class Scene;

class SceneArchitectureManager : public Identifiable
{
public:
  SceneArchitectureManager(Scene* scene);

  ~SceneArchitectureManager(void);

  void add(ArchitectureDesc description, Vector3 position = Vector3(0,0,0), Quaternion quaternion = Quaternion::IDENTITY, Vector3 scale = Vector3::UNIT_SCALE);//adds a static mesh to the current dungeon architecture

  void build();

private:
  Scene* scene;

  Ogre::SceneNode* rootNode;

  bool isBuilt;//true if build has been called
  
  int instanceNumber;//current number of physics items

  std::vector<Ogre::SceneNode*> nodes;
  std::map<Ogre::String, physx::PxTriangleMesh*> statics;
  std::vector<physx::PxRigidStatic*> actors;

  Ogre::StaticGeometry* geometry;

  void addStaticTrimesh(std::string meshName, float restitution, float friction, Vector3 position, Quaternion quaternion, Vector3 scale = Vector3::UNIT_SCALE);

  float defaultFriction;
  float defaultRestitution;
};

