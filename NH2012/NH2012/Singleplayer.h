#pragma once

#include <OgreFrameListener.h>

#include "Client.h"

/**
 * Singleplayer frame generation.
 * Far simpler than multiplayer, since rather than threading 
 * frame generation and rendering, whenever a frame is requested, 
 * it is generated.
 */
class Singleplayer : 
  public Client
{
public:
  Singleplayer(void);
  ~Singleplayer(void);

  void generateFrame();
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};

