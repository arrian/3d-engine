#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include <OgreVector3.h>

#include "Inventory.h"
#include "Attributes.h"
#include "Bar.h"

class Monster
{
public:
  Monster(int difficulty);
  virtual ~Monster(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
  Ogre::Vector3 position;

  Ogre::String name;

  Bar health;
  Bar magic;

  int level;

  Inventory inventory;
  Attributes attributes;

};

