#include "Interface.h"

#include "World.h"

Interface::Interface(World* world)
  : overlay(new Gorilla::Silverback),
    world(world),
    screen(NULL),
    consoleScreen(world),
    menuScreen(world),
    loadingScreen(world),
    playerScreen(world),
    display(&playerScreen)
{
  overlay->loadAtlas("dejavu");
}

//-------------------------------------------------------------------------------------
Interface::~Interface(void)
{
  delete overlay;
}

//-------------------------------------------------------------------------------------
void Interface::update(double elapsedSeconds) 
{
  display->update(elapsedSeconds);
}

//-------------------------------------------------------------------------------------
void Interface::hookWindow(Ogre::RenderWindow* window) 
{
  if(screen) overlay->destroyScreen(screen);

  this->window = window;
  screen = overlay->createScreen(window->getViewport(0), "dejavu");
  
  playerScreen.setLayer(screen->createLayer(0U));
  menuScreen.setLayer(screen->createLayer(1U));
  loadingScreen.setLayer(screen->createLayer(2U));
  consoleScreen.setLayer(screen->createLayer(3U));
}

//-------------------------------------------------------------------------------------
void Interface::displayMenuScreen() 
{
  display = &menuScreen;
}

//-------------------------------------------------------------------------------------
void Interface::displayPlayerScreen() 
{
  display = &playerScreen;
  consoleScreen.setVisible(false);
}

//-------------------------------------------------------------------------------------
void Interface::displayLoadingScreen() 
{
  display = &loadingScreen;
}

//-------------------------------------------------------------------------------------
void Interface::displayConsoleScreen() 
{
  display = &consoleScreen;
  consoleScreen.setVisible(true);
}

//-------------------------------------------------------------------------------------
bool Interface::capturedInput() 
{
  return consoleScreen.isVisible() || menuScreen.isVisible() || loadingScreen.isVisible();
}

//-------------------------------------------------------------------------------------
void Interface::keyPressed(const OIS::KeyEvent &arg) 
{
  display->keyPressed(arg);
}

//-------------------------------------------------------------------------------------
void Interface::keyReleased(const OIS::KeyEvent &arg) 
{
  if(arg.key == world->getControlManager()->console)//display or hide the console
  {
    if(consoleScreen.isVisible()) displayPlayerScreen();
    else displayConsoleScreen();
  }
  display->keyReleased(arg);
}

//-------------------------------------------------------------------------------------
void Interface::mouseMoved(const OIS::MouseEvent &arg) 
{
  display->mouseMoved(arg);
}

//-------------------------------------------------------------------------------------
void Interface::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) 
{
  display->mousePressed(arg, id);
}

//-------------------------------------------------------------------------------------
void Interface::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) 
{
  display->mouseReleased(arg, id);
}


