#include "Singleplayer.h"


Singleplayer::Singleplayer(Ogre::Root* root, Ogre::RenderWindow* renderWindow, OIS::Keyboard* keyboard) 
  : Game(renderWindow),
    world(root)//,
    //console(&world, keyboard)
{
  try
  {
    world.initialise("C:\\Dev\\Nethack2012\\NH2012\\Media\\nh2012.ini");
    Console::getInstance().print("Initialised.");
    world.hookWindow(renderWindow);
    Console::getInstance().setRequired(&world, keyboard);
    Console::getInstance().hookWindow(renderWindow);
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

void Singleplayer::update(double elapsedSeconds)
{
  try
  {
    Console::getInstance().update(elapsedSeconds);
    //console.update(elapsedSeconds);
    world.update(elapsedSeconds);
  }
  catch (NHException e)//temporarily catching update errors
  {
    std::cout << e.what() << std::endl;
  }
}

void Singleplayer::injectKeyDown(const OIS::KeyEvent &arg)
{
  Console::getInstance().injectKeyDown(arg);
  //console.injectKeyDown(arg);
  if(Console::getInstance().isVisible()) return;//ignore other key notifications while console visible

  world.injectKeyDown(arg);
}

void Singleplayer::injectKeyUp(const OIS::KeyEvent &arg)
{
  try
  {
    if(arg.key == world.controls.console) Console::getInstance().setVisible(!Console::getInstance().isVisible());
    Console::getInstance().injectKeyUp(arg);
    if(Console::getInstance().isVisible()) return;//ignore other key notifications while console visible

    world.injectKeyUp(arg);

  }
  catch (NHException e)//temporarily catching key up errors
  {
    std::cout << e.what() << std::endl;
  }
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
  Console::getInstance().print(comment);
}

void Singleplayer::sceneChanged()
{
  world.hookWindow(renderWindow);
  Console::getInstance().hookWindow(renderWindow);//reconnect the console to the scene
}


