#pragma once

#include <vector>

#include <OgreString.h>
#include <OgreVector3.h>

#include "OgreBulletCollisionsShape.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsConeShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "Shapes/OgreBulletCollisionsTriangleShape.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsCompoundShape.h"
#include "Shapes/OgreBulletCollisionsMultiSphereShape.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsMinkowskiSumShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "OgreBulletCollisionsRay.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"
#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsConstraint.h"
#include "Constraints/OgreBulletDynamicsPoint2pointConstraint.h" 

class Architecture
{
public:
  Architecture(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics);
  ~Architecture(void);

  /*! Adds a static mesh to the current dungeon architecture.*/
  void add(Ogre::String meshName, Ogre::Vector3 position = Ogre::Vector3(0,0,0), Ogre::Quaternion quaternion = Ogre::Quaternion::IDENTITY);

  void build();
private:
  /*! Current number of physics items.*/
  int instanceNumber;

  /*! Pointers to the ogre scene nodes.*/
  std::vector<Ogre::SceneNode*> nodes;

  std::map<Ogre::String, Ogre::Entity*> entities;
  //std::vector<Ogre::Entity*> entities;
  std::vector<OgreBulletDynamics::RigidBody*> bodies;
  std::vector<OgreBulletCollisions::CollisionShape*> shapes;

  OgreBulletDynamics::DynamicsWorld* physics;
  Ogre::SceneManager* sceneManager;

  OgreBulletDynamics::RigidBody* addStaticTrimesh(Ogre::String meshName, Ogre::Real restitution, const Ogre::Real friction, Ogre::Vector3 position, Ogre::Quaternion quaternion);

  Ogre::StaticGeometry* geometry;
};

