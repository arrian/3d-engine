#pragma once

#include <OgreVector3.h>

#include "Actor.h"
#include "Bar.h"
#include "Inventory.h"
#include "Attributes.h"
#include "Dungeon.h"

class Player : public Actor
{
public:
  Player(Ogre::SceneManager* sceneManager, Dungeon* dungeon, Ogre::Vector3 position);
  ~Player(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
  //hardcore mode
  Bar water;
  Bar food;
  Bar sleep;

  Dungeon* currentDungeon;

  void animation();
  void audio();
  void collision();
};

