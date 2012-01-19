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
  Multiplayer(void);
  ~Multiplayer(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
  Client* client;

  boost::thread listener;
  void listen(const std::string& host, unsigned short port);
};

