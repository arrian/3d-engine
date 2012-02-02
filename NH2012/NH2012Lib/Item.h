#pragma once

#include <OgreString.h>

#include "Entity.h"
#include "ItemData.h"

class Item : public Entity, public ItemData
{
public:
  Item(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
       Ogre::Vector3 position = Ogre::Vector3(0,0,0), int id = 0);
  virtual ~Item(void);

  
private:
  OgreBulletCollisions::BoxCollisionShape* physicsShape;
  OgreBulletDynamics::RigidBody* physicsBody;
};

