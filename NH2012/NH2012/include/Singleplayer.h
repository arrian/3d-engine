#pragma once

#include <OgreFrameListener.h>

#include "Game.h"
#include "../../Engine/Console.h"

/*! Singleplayer frame generation.
    Far simpler than multiplayer, since rather than threading 
    frame generation and rendering, whenever a frame is requested, 
    it is generated.
*/
class Singleplayer : 
  public Game
{
public:
  Singleplayer(Ogre::Root* root, Ogre::RenderWindow* window, OIS::Keyboard* keyboard);
  ~Singleplayer(void);

  void update(double elapsedSeconds);
  void setSceneManager(Ogre::SceneManager* scene);

  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  void notify(Ogre::String comment);

  void sceneChanged();

private:
  World world;
  Console console;
};

