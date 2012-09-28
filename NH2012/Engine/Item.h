#pragma once

#include <OgreString.h>

#include "PhysicalInterface.h"
#include "VisualComponent.h"
#include "PhysicalComponent.h"
#include "BasicComponent.h"
#include "DataManager.h"

#include "PxPhysicsAPI.h"

class Scene;

class Item : public BasicComponent, public PhysicalInterface
{
public:
  Item(ItemDesc description);
  virtual ~Item(void);

  friend bool operator==(const Item& x, const Item& y);

  void update(double elapsedSeconds);

  void setPosition(Ogre::Vector3 position);
  void setRotation(Ogre::Quaternion rotation);

  Ogre::Vector3 getPosition();
  Ogre::Quaternion getRotation();

protected:
  std::string mesh;
  Ogre::Entity* entity;

  Ogre::SceneNode* node;
  Ogre::Vector3 position;
  Ogre::Quaternion rotation;

  void hasSceneChange();

  //VisualComponent visual;
  //PhysicalComponent physical;

  void loadPhysical();//loads a simplified collision mesh
  void mapPhysical(void* target);

  Ogre::Real friction;
  Ogre::Real restitution;
  physx::PxMaterial* material;
  physx::PxRigidDynamic* physical;
  physx::PxShape* shape;
  float tempCollisionCubeSides;
  float tempCollisionDensity;
};

