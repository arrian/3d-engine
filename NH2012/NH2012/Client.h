#pragma once

#include <OgreFrameListener.h>

class Client
{
public:
  Client();
  virtual ~Client(void);

  virtual void generateFrame() = 0;
  virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;
};

