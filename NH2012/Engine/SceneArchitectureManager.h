#pragma once

#include <vector>

#include <OgreString.h>
#include "Vector3.h"
#include <OgreSceneNode.h>

#include "Identifiable.h"
#include "DataManager.h"
#include "Architecture.h"
#include "NHException.h"

#include "PxPhysicsAPI.h"

class Scene;

class SceneArchitectureManager : public Identifiable
{
public:
  SceneArchitectureManager(Scene* scene);
  ~SceneArchitectureManager(void);

  void add(Id<Architecture> instanceId, std::shared_ptr<Architecture> a) 
  {
    architecture.insert(instanceId, a);
    add(a->description, a->position, a->quaternion, a->scale);
  }

  std::shared_ptr<Architecture> remove(Id<Architecture> instanceId)
  {
    throw NHException("removing architecture not implemented");
    return std::shared_ptr<Architecture>();
  }

  void build();

private:
  Scene* scene;

  Ogre::SceneNode* rootNode;

  bool isBuilt;//true if build has been called
  
  int instanceNumber;//current number of physics items

  Container<Architecture> architecture;
  std::vector<Ogre::SceneNode*> nodes;
  std::vector<physx::PxRigidStatic*> actors;
  std::vector<physx::PxShape*> shapes;
  std::vector<Ogre::Entity*> entities;

  Ogre::StaticGeometry* geometry;

  void add(ArchitectureDesc description, Vector3 position = Vector3(0,0,0), Quaternion quaternion = Quaternion::IDENTITY, Vector3 scale = Vector3::UNIT_SCALE);//adds a static mesh to the current dungeon architecture
  void addStaticTrimesh(std::string meshName, float restitution, float friction, Vector3 position, Quaternion quaternion, Vector3 scale = Vector3::UNIT_SCALE);

  float defaultFriction;
  float defaultRestitution;
};

