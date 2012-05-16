#include "Singleplayer.h"


Singleplayer::Singleplayer(Ogre::Root* root, Ogre::RenderWindow* renderWindow, OIS::Keyboard* keyboard) 
  : Game(renderWindow),
    world(root),
    console(&world, keyboard)
{
  try
  {
    world.initialise("C:\\Dev\\Nethack2012\\NH2012\\Media\\nh2012.ini");
    world.hookWindow(renderWindow);
    console.hookWindow(renderWindow);
    world.setSceneChangeListener(this);//listens for a scene change. used for reconnecting the console to the render window.
  }
  catch (NHException e)
  {
  	std::cout << e.what() << std::endl;
  }
}

Singleplayer::~Singleplayer(void)
{
}

void Singleplayer::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  console.frameRenderingQueued(evt);
  world.frameRenderingQueued(evt);
}

void Singleplayer::injectKeyDown(const OIS::KeyEvent &arg)
{
  console.injectKeyDown(arg);
  if(console.isVisible()) return;//ignore other key notifications while console visible

  world.injectKeyDown(arg);
}

void Singleplayer::injectKeyUp(const OIS::KeyEvent &arg)
{
  if(arg.key == world.controls.console) console.setVisible(!console.isVisible());
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

void Singleplayer::sceneChanged()
{
  console.hookWindow(renderWindow);//reconnect the console to the scene
}


