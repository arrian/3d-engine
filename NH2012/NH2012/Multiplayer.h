#pragma once

#include <OgreFrameListener.h>
#include <boost/thread.hpp>

#include "Client.h"

#include <World.h>
 
struct PacketSocket
{
  PacketSocket(int id) : id(id) { }
 
  void operator()()
  {
    int i = 0;
    while(true)
    {
      i++;
      std::cout << id << ": " << i << std::endl;
    }
  }
 
  int id;
};

class Multiplayer :
  public Client
{
public:
  Multiplayer(void);
  ~Multiplayer(void);

  WorldData convertResponse();
  WorldData frameRenderingQueued(const Ogre::FrameEvent& evt);
private:
  int response;

  boost::thread packetSocket;
};

