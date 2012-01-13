#pragma once

#include <OgreFrameListener.h>

#include <World.h>

class Client
{
public:
  Client();
  virtual ~Client(void);

  virtual void generateFrame() = 0;
  virtual WorldData frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;
};

