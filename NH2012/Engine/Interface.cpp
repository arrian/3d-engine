#include "Interface.h"

#include "World.h"

Interface::Interface(World* world)
  : overlay(new Gorilla::Silverback),
    world(world),
    screen(NULL),
    menuScreen(world),
    consoleScreen(world),
    loadingScreen(world),
    playerScreen(world),
    cursorScreen(world),
    display(NULL)
{
  displayPlayerScreen();//displayMenuScreen();
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
  if(cursorScreen.isVisible()) cursorScreen.update(elapsedSeconds);
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
  cursorScreen.setLayer(screen->createLayer(4U));
}

//-------------------------------------------------------------------------------------
void Interface::resizeWindow(Ogre::Vector2 dimensions)
{
  playerScreen.setSize(dimensions);
  menuScreen.setSize(dimensions);
  loadingScreen.setSize(dimensions);
  consoleScreen.setSize(dimensions);
  cursorScreen.setSize(dimensions);
}

//-------------------------------------------------------------------------------------
void Interface::displayMenuScreen() 
{
  setDisplay(&menuScreen);
  cursorScreen.setVisible(true);
}

//-------------------------------------------------------------------------------------
void Interface::displayPlayerScreen() 
{
  setDisplay(&playerScreen);
  cursorScreen.setVisible(false);
}

//-------------------------------------------------------------------------------------
void Interface::displayLoadingScreen() 
{
  setDisplay(&loadingScreen);
  cursorScreen.setVisible(false);
}

//-------------------------------------------------------------------------------------
void Interface::displayConsoleScreen() 
{
  setDisplay(&consoleScreen);
  cursorScreen.setVisible(true);
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
  if(cursorScreen.isVisible()) cursorScreen.mouseMoved(arg);
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

//-------------------------------------------------------------------------------------
void Interface::setDisplay(Screen* display)
{
  if(this->display) this->display->setVisible(false);
  this->display = display;
  if(!display) return;
  this->display->setVisible(true);
}


