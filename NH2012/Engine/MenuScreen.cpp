#include "MenuScreen.h"

#include "InterfaceButton.h"

MenuScreen::MenuScreen(World* world)
  : Screen(world),
    buttons()
{
  //buttons.push_back(new InterfaceButton<MenuScreen>("Resume", Ogre::Vector2(0,0), Ogre::Vector2(200,75), this, &resume));
  buttons.push_back(new InterfaceButton<MenuScreen>("Start", Ogre::Vector2(0,80), Ogre::Vector2(200,75), this, &MenuScreen::start));
  buttons.push_back(new InterfaceButton<MenuScreen>("Options", Ogre::Vector2(0,160), Ogre::Vector2(200,75), this, &MenuScreen::options));
  buttons.push_back(new InterfaceButton<MenuScreen>("Exit", Ogre::Vector2(0,240), Ogre::Vector2(200,75), this, &MenuScreen::exit));

}


MenuScreen::~MenuScreen(void)
{
  for(std::vector<InterfaceButton<MenuScreen>*>::iterator iter = buttons.begin(); iter != buttons.end(); ++iter)
  {
    delete (*iter);
  }
}

void MenuScreen::mouseMoved(const OIS::MouseEvent &arg)
{
  for(std::vector<InterfaceButton<MenuScreen>*>::iterator iter = buttons.begin(); iter != buttons.end(); ++iter)
  {
    (*iter)->mouseMoved(arg);
  }
}

void MenuScreen::start()
{

}

void MenuScreen::resume()
{

}

void MenuScreen::options()
{

}

void MenuScreen::exit()
{
  
}

void MenuScreen::hasLayerChange()
{
  for(std::vector<InterfaceButton<MenuScreen>*>::iterator iter = buttons.begin(); iter != buttons.end(); ++iter)
  {
    (*iter)->setLayer(layer);
  }
}



