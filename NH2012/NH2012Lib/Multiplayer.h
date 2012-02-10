#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <OgreFrameListener.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include "Game.h"

#include <World.h>
#include <Client.h>
 
class Multiplayer :
  public Game
{
public:
  Multiplayer(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window);
  ~Multiplayer(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
private:
  Client* client;

  boost::thread listener;
  void listen(const std::string& host, unsigned short port);
};

