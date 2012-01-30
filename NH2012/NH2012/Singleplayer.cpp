#include "Singleplayer.h"


Singleplayer::Singleplayer(Ogre::SceneManager* sceneManager) : 
  Game(),
  world(sceneManager)
{
  std::cout << "Moon phase : " << world.flags.getMoonPhase() << std::endl;
  frame = 0;
}

Singleplayer::~Singleplayer(void)
{
}

void Singleplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  world.frameRenderingQueued(evt);
  frame++;
  if(frame > 1000) frame = 0;
}