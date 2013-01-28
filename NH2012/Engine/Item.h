#pragma once

#include <OgreString.h>
#include "Quaternion.h"

#include "Identifiable.h"
#include "MeshComponent.h"
#include "PhysicalComponent.h"
#include "BasicComponent.h"
#include "DataManager.h"
#include "Group.h"

#include "PxPhysicsAPI.h"
#include "Packetable.h"

class Scene;

class Item : public BasicComponent, public Identifiable, public Packetable<ItemPacket>
{
public:
  Item(ItemDesc description);
  virtual ~Item(void);

  void update(double elapsedSeconds);

  //Setters
  virtual void setPosition(Vector3 position);
  virtual void setRotation(Quaternion rotation);
  virtual void setVelocity(Vector3 velocity);

  //Getters
  virtual Vector3 getPosition();
  virtual Quaternion getRotation();
  virtual Vector3 getVelocity();
  ItemDesc getDescription();

  //Network
  virtual void integratePacket(ItemPacket packet);
  virtual ItemPacket extractPacket();

protected:
  ItemDesc desc;
  Ogre::SceneNode* node;
  Ogre::Entity* entity;
  Ogre::Entity* simplifiedEntity;
  PhysicalComponent physical;
  physx::PxMaterial* material;
  
  void hasSceneChange();
  void setUserData(void* target);
};

