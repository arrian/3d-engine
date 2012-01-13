#include "Singleplayer.h"


Singleplayer::Singleplayer(void)
{
}


Singleplayer::~Singleplayer(void)
{
}

void Singleplayer::generateFrame()
{

}

WorldData Singleplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  world->frameRenderingQueued(evt);
  return world->getWorldState();
}