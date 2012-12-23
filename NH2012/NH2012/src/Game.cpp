#include "Game.h"


Game::Game(Ogre::Root* root, Ogre::RenderWindow* renderWindow, OIS::Keyboard* keyboard) 
  : renderWindow(renderWindow),
    world(root),
    done(false)
{
  try
  {
    world.initialise("nh2012.ini");
    Console::getInstance().print("Initialised.");
    world.hookWindow(renderWindow);
    Console::getInstance().setWorld(&world);
    Console::getInstance().setKeyboard(keyboard);
    Console::getInstance().hookWindow(renderWindow);
    world.setSceneChangeListener(&Console::getInstance());//listens for a scene change. used for reconnecting the console to the render window.
  }
  catch (NHException e)//temporarily catching initialisation errors
  {
    std::cout << "Error: " << e.what() << std::endl;
  }
}

//-------------------------------------------------------------------------------------
Game::~Game(void)
{
}

//-------------------------------------------------------------------------------------
bool Game::update(double elapsedSeconds)
{
  if(done) return false;
  try
  {
    Console::getInstance().update(elapsedSeconds);
    if(!world.update(elapsedSeconds)) return false;
  }
  catch (NHException e)//temporarily catching update errors
  {
    std::cout << "Error: " << e.what() << std::endl;
  }
  return !done;//should update is not of done
}

//-------------------------------------------------------------------------------------
bool Game::keyPressed(const OIS::KeyEvent &arg)
{
  if (arg.key == OIS::KC_ESCAPE) done = true;

  Console::getInstance().injectKeyDown(arg);
  if(Console::getInstance().isVisible()) return true;//ignore other key notifications while console visible

  world.injectKeyDown(arg);

  return true;
}

//-------------------------------------------------------------------------------------
bool Game::keyReleased(const OIS::KeyEvent &arg)
{
  try
  {
    if(arg.key == world.getControlManager()->console) Console::getInstance().setVisible(!Console::getInstance().isVisible());
    Console::getInstance().injectKeyUp(arg);
    if(Console::getInstance().isVisible()) return true;//ignore other key notifications while console visible

    world.injectKeyUp(arg);
  }
  catch (NHException e)//temporarily catching key up errors
  {
    std::cout << "Error: " << e.what() << std::endl;
  }

  return true;
}

//-------------------------------------------------------------------------------------
bool Game::mouseMoved(const OIS::MouseEvent &arg)
{
  world.injectMouseMove(arg);

  return true;
}

//-------------------------------------------------------------------------------------
bool Game::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  world.injectMouseDown(arg,id);

  return true;
}

//-------------------------------------------------------------------------------------
bool Game::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  world.injectMouseUp(arg,id);
  
  return true;
}




