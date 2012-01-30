#pragma once

#include <vector>

#include <OgreFrameListener.h>

#include "Dungeon.h"
#include "Flag.h"
#include "Player.h"

class World
{
public:
  World(Ogre::SceneManager* sceneManager);
  ~World(void);

  Flag flags;

  void setSceneManager(Ogre::SceneManager* sceneManager);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations
private:
  Ogre::SceneManager* sceneManager;

  int timestamp;

  std::vector<Dungeon> dungeons;
  std::vector<Player> players;
};

