#pragma once

#include <OgreFrameListener.h>

#include "Client.h"

class Multiplayer :
  public Client
{
public:
  Multiplayer(void);
  ~Multiplayer(void);

  void generateFrame();
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

