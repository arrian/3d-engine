#pragma once

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

#include <OgreRoot.h>
#include <OgreConfigFile.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreFrameListener.h>

#include <OISMouse.h>
#include <OISEvents.h>
#include <OISKeyboard.h>
#include <OISInputManager.h>

#include <Interface.h>
#include <World.h>

#include <boost/shared_ptr.hpp>


//Dump memory leaks to standard out
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


class Game : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
  Game(void);
  virtual ~Game(void);
  bool run();

protected:
  bool done;
  boost::shared_ptr<World> world;
  boost::shared_ptr<Interface> userInterface;
  boost::shared_ptr<Ogre::Root> root;

  Ogre::RenderWindow* window;

  OIS::InputManager* inputManager;
  OIS::Keyboard* keyboard;
  OIS::Mouse* mouse;
  
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);

  virtual void windowResized(Ogre::RenderWindow* rw);
  virtual void windowClosed(Ogre::RenderWindow* rw);

  bool keyPressed(const OIS::KeyEvent &arg);
  bool keyReleased(const OIS::KeyEvent &arg);
  bool mouseMoved(const OIS::MouseEvent &arg);
  bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
};

