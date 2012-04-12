#pragma once

#include <string>

#include <OgreString.h>
#include <OgreSceneManager.h>
#include <OgreEntity.h>



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


class Entity
{
public:
  Entity(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
         Ogre::String meshName = Ogre::String("default.mesh"), Ogre::Vector3 position = Ogre::Vector3(0,0,0), 
         int id = 0, Ogre::String name = Ogre::String("DefaultEntity"));
  ~Entity(void);
protected:
  int id;
  Ogre::String entityName;

  Ogre::Entity* entity;
  Ogre::SceneNode* node;

  OgreBulletDynamics::DynamicsWorld* physics;
};

