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

bool Singleplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  generateFrame();
  return true;
}