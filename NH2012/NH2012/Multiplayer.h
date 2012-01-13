#pragma once

#include <OgreFrameListener.h>
#include <boost/thread.hpp>

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <iostream>

#include "Client.h"

#include <World.h>


struct FrameRequester
{
FrameRequester() { }
void operator()()
{
boost::xtime xt;
boost::xtime_get(&xt, boost::TIME_UTC);
xt.sec += 1;

boost::thread::sleep(xt);

std::cout << "alarm sounded..." << std::endl;
}

int timestamp;
};


class Multiplayer :
  public Client
{
public:
  Multiplayer(void);
  ~Multiplayer(void);

  void generateFrame();
  WorldData convertResponse();
  WorldData frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
  int response;
};

