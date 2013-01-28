#include "Screen.h"


Screen::Screen(World* world)
  : layer(NULL),
    visible(false),
    world(world)
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

void Screen::setWorld(World* world)
{
  this->world = world;
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