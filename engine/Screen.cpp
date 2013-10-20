#include "Screen.h"


Screen::Screen()
  : layer(NULL),
    visible(false)
{
}


Screen::~Screen(void)
{
}

void Screen::setLayer(Gorilla::Layer* layer) 
{
  this->layer = layer;
  layer->setVisible(visible);
  hasLayerChange();
}

void Screen::setVisible(bool visible)
{
  this->visible = visible;
  if(layer) layer->setVisible(visible);
}

bool Screen::isVisible() 
{
  return visible;
}

void Screen::setSize(Ogre::Vector2 dimensions)
{
  this->dimensions = dimensions;
  hasSizeChange();
}

Ogre::Vector2 Screen::getSize()
{
  return dimensions;
}