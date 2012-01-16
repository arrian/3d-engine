#pragma once

#include <OgreFrameListener.h>

#include <World.h>

class Game
{
public:
  Game();
  virtual ~Game(void);

  virtual WorldData frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;
};

