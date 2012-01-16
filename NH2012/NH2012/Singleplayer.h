#pragma once

#include <OgreFrameListener.h>

#include <World.h>

#include "Game.h"

/**
 * Singleplayer frame generation.
 * Far simpler than multiplayer, since rather than threading 
 * frame generation and rendering, whenever a frame is requested, 
 * it is generated.
 */
class Singleplayer : 
  public Game
{
public:
  Singleplayer(void);
  ~Singleplayer(void);

  WorldData frameRenderingQueued(const Ogre::FrameEvent& evt);

  World* world;
};

