#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include <OgreVector3.h>

#include "Actor.h"
#include "Dungeon.h"
#include "Inventory.h"
#include "Attributes.h"
#include "Bar.h"

class Dungeon;//forward declaring for circular dependency

class Monster : public Actor
{
public:
  Monster(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
          Ogre::Vector3 position, int id = 0, int difficulty = 1);
  virtual ~Monster(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void setTarget(Ogre::Vector3 target);
private:
  Ogre::Vector3 target;

  void animation();
  void audio();
  void collision();
};

