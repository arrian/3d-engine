#pragma once

#include <vector>

#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreFrameListener.h>

#include "Dungeon.h"
#include "Flag.h"
#include "Player.h"

class World
{
public:
  World(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window);
  ~World(void);

  Flag flags;

  void setSceneManager(Ogre::SceneManager* sceneManager);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations

  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
private:
  Ogre::SceneManager* sceneManager;

  int timestamp;

  std::vector<Dungeon> dungeons;

  Player player;
  //std::vector<Player> players;
};

