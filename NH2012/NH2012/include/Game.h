#pragma once

#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreFrameListener.h>

#include "Game.h"
#include "../../Engine/Console.h"
#include "../../Engine/World.h"


class Game : public OIS::KeyListener, public OIS::MouseListener
{
public:
  Game(Ogre::Root* root, Ogre::RenderWindow* renderWindow, OIS::Keyboard* keyboard);
  virtual ~Game(void);

  bool update(double elapsedSeconds);
  void setSceneManager(Ogre::SceneManager* scene);

  bool keyPressed(const OIS::KeyEvent &arg);
  bool keyReleased(const OIS::KeyEvent &arg);
  bool mouseMoved(const OIS::MouseEvent &arg);
  bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

protected:
  bool done;
  World world;
  Ogre::RenderWindow* renderWindow;
};



