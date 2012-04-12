#pragma once

#include <OgreFrameListener.h>

#include "Game.h"

/*! Singleplayer frame generation.
    Far simpler than multiplayer, since rather than threading 
    frame generation and rendering, whenever a frame is requested, 
    it is generated.
*/
class Singleplayer : 
  public Game
{
public:
  Singleplayer(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window);
  ~Singleplayer(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);
  void setSceneManager(Ogre::SceneManager* scene);

  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  World world;
};

