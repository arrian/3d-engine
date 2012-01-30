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
  Singleplayer(Ogre::SceneManager* sceneManager);
  ~Singleplayer(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);
  void setSceneManager(Ogre::SceneManager* scene);

  World world;
};

