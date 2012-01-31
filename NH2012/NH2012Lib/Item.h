#pragma once

#include <OgreString.h>

#include "Entity.h"
#include "Attributes.h"

class Item : public Entity
{
public:
  Item(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
       Ogre::Vector3 position = Ogre::Vector3(0,0,0), int id = 0);
  virtual ~Item(void);

  friend bool operator==(const Item& x, const Item& y);
private:
  int value;

  ItemAttributes attributes;
};

