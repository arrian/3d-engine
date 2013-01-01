#pragma once

#include <vector>

#include <OgreString.h>
#include "Vector3.h"
#include <OgreSceneNode.h>

#include "IdentificationInterface.h"
#include "PathfindManager.h"
#include "DataManager.h"

#include "PxPhysicsAPI.h"

class Scene;

/*
struct StaticEntity
{
  Ogre::Entity* entity;
  physx::PxTriangleMesh* mesh;

  StaticEntity(Ogre::Entity* entity, physx::PxTriangleMesh* mesh)
    : entity(entity),
      mesh(mesh)
  {
  }
};*/

class Architecture : public IdentificationInterface
{
public:
  Architecture(Scene* scene, PathfindManager* pathfinder = NULL);

  ~Architecture(void);

  /*! Adds a static mesh to the current dungeon architecture.*/
  void add(ArchitectureDesc description, Vector3 position = Vector3(0,0,0), Quaternion quaternion = Quaternion::IDENTITY, Vector3 scale = Vector3::UNIT_SCALE);

  void build();

  void update(double elapsedSeconds);
private:
  Scene* scene;

  Ogre::SceneNode* rootNode;

  bool isBuilt;//true if build has been called
  
  /*! Current number of physics items.*/
  int instanceNumber;

  std::vector<Ogre::SceneNode*> nodes;
  std::map<Ogre::String, physx::PxTriangleMesh*> statics;
  std::vector<physx::PxRigidStatic*> actors;

  Ogre::StaticGeometry* geometry;

  void addStaticTrimesh(Ogre::String meshName, float restitution, float friction, Vector3 position, Quaternion quaternion, Vector3 scale = Vector3::UNIT_SCALE);

  float defaultFriction;
  float defaultRestitution;
  
  PathfindManager* pathfinder;
};

