#pragma once

#include <OgreFrameListener.h>
#include <OgreVector3.h>
#include <OgreSceneManager.h>

#include "Bar.h"
#include "Inventory.h"
#include "Attributes.h"
#include "Entity.h"

class Actor : public Entity
{
public:
  Actor(Ogre::SceneManager* sceneManager, Ogre::Vector3 position = Ogre::Vector3(0,0,0));
  ~Actor(void);

  virtual void frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;
protected:
  Ogre::Real speed;//number of units to move per second ~100 = 1 metre per second

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

