#include "Singleplayer.h"


Singleplayer::Singleplayer(Ogre::Root* root, Ogre::RenderWindow* window, OIS::Keyboard* keyboard) : 
  Game(),
  world(root, window),
  console(root, window, keyboard, &world)
{
}

Singleplayer::~Singleplayer(void)
{
}

void Singleplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  console.frameRenderingQueued(evt);
  world.frameRenderingQueued(evt);
}

//TODO find solution to chaining of events between classes

void Singleplayer::injectKeyDown(const OIS::KeyEvent &arg)
{
  console.injectKeyDown(arg);
  if(console.isVisible()) return;//ignore other key notifications while console visible

  world.injectKeyDown(arg);
}

void Singleplayer::injectKeyUp(const OIS::KeyEvent &arg)
{
  if(arg.key == world.getEnvironment()->controls.console) console.setVisible(!console.isVisible());
  console.injectKeyUp(arg);
  if(console.isVisible()) return;//ignore other key notifications while console visible

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

void Singleplayer::notify(Ogre::String comment)
{
  console.display(comment);
}
