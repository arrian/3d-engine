#include "CursorScreen.h"


CursorScreen::CursorScreen(World* world)
  : Screen(world)
{
}


CursorScreen::~CursorScreen(void)
{
}

void CursorScreen::hasLayerChange()
{
  if(!layer) return;
  cursorImage = layer->createRectangle(Ogre::Vector2(0,0), Ogre::Vector2(20,20));
  cursorImage->background_colour(Ogre::ColourValue(1.0,0.0,0.0));
}

void CursorScreen::mouseMoved(const OIS::MouseEvent &arg)
{
  cursorImage->position(Ogre::Vector2(arg.state.X.abs, arg.state.Y.abs));
}


