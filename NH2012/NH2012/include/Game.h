#pragma once

#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreFrameListener.h>

#include "../../Engine/World.h"

class Game
{
public:
  Game(void);
  virtual ~Game(void);

  virtual void frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;

  virtual void injectKeyDown(const OIS::KeyEvent &arg) = 0;
  virtual void injectKeyUp(const OIS::KeyEvent &arg) = 0;
  virtual void injectMouseMove(const OIS::MouseEvent &arg) = 0;
  virtual void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
  virtual void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0;
};

