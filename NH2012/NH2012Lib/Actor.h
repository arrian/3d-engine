#pragma once

#include <OgreFrameListener.h>
#include <OgreVector3.h>

#include "Bar.h"
#include "Inventory.h"
#include "Attributes.h"
#include "Entity.h"

class Actor : public Entity
{
public:
  Actor(void);
  ~Actor(void);

  virtual void frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;
protected:
  Ogre::Vector3 position;
  Ogre::Real speed;

  Bar level;
  Bar health;
  Bar magic;

  int magicResistance;
  int meleeResistance;

  Inventory inventory;
  MonsterAttributes attributes;

  virtual void animation() = 0;
  virtual void audio() = 0;
  virtual void collision() = 0;
};

