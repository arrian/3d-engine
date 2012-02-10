#include "Singleplayer.h"


Singleplayer::Singleplayer(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window) : 
  Game(),
  world(sceneManager, window)
{
  std::cout << "Moon phase : " << world.flags->getMoonPhase() << std::endl;
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

//TODO find solution to chaining of events between classes

void Singleplayer::injectKeyDown(const OIS::KeyEvent &arg)
{
  world.injectKeyDown(arg);
}

void Singleplayer::injectKeyUp(const OIS::KeyEvent &arg)
{
  world.injectKeyUp(arg);
}

void Singleplayer::injectMouseMove(const OIS::MouseEvent &arg)
{
  world.injectMouseMove(arg);
}

void Singleplayer::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  world.injectMouseDown(arg,id);
}

void Singleplayer::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  world.injectMouseUp(arg,id);
}
