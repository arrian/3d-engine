#include "Singleplayer.h"


Singleplayer::Singleplayer(void)
{
  //world = new World();
}


Singleplayer::~Singleplayer(void)
{
}

void Singleplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  frame++;
}