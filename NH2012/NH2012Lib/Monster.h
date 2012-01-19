#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include <OgreVector3.h>

#include "World.h"
#include "Inventory.h"
#include "Attributes.h"
#include "Bar.h"

class World;//forward declaring for circular dependency

class Monster
{
public:
  Monster(int difficulty);
  virtual ~Monster(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void setTarget(Ogre::Vector3 target);
private:
  int id;//unique identifier for instance
  Ogre::String name;

  Ogre::Vector3 position;
  Ogre::Vector3 target;

  Ogre::Real speed;

  Bar level;
  Bar health;
  Bar magic;

  Inventory inventory;
  MonsterAttributes attributes;

  void fixCollision(World* world);
  void fixAI(World* world);
};

