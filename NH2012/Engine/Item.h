#pragma once

#include <OgreString.h>
#include <OgreQuaternion.h>

#include "IdentificationInterface.h"
#include "MeshComponent.h"
#include "PhysicalComponent.h"
#include "BasicComponent.h"
#include "DataManager.h"
#include "Group.h"

#include "PxPhysicsAPI.h"

class Scene;

class Item : public BasicComponent, public IdentificationInterface
{
public:
  Item(ItemDesc description);
  virtual ~Item(void);

  friend bool operator==(const Item& x, const Item& y);

  void update(double elapsedSeconds);

  virtual void setPosition(Ogre::Vector3 position);
  virtual void setRotation(Ogre::Quaternion rotation);
  virtual void setVelocity(Ogre::Vector3 velocity);

  virtual Ogre::Vector3 getPosition();
  virtual Ogre::Quaternion getRotation();
  virtual Ogre::Vector3 getVelocity();

  ItemDesc getDescription();

protected:
  ItemDesc desc;
  Ogre::Entity* entity;
  Ogre::Entity* simplifiedEntity;

  Ogre::SceneNode* node;

  void hasSceneChange();

  void loadPhysical();//loads a simplified collision mesh
  void setUserData(void* target);
  void setGroup(Group group);

  physx::PxMaterial* material;
  physx::PxRigidDynamic* physical;
  physx::PxShape* shape;
};

