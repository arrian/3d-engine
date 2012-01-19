#pragma once

#include <OgreFrameListener.h>

#include <World.h>

class Game
{
public:
  Game();
  virtual ~Game(void);

  virtual void frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;

  int frame;
};

