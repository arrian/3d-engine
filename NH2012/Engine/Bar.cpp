#include "Bar.h"

Bar::Bar()
{
  this->max = 0;
  current = max;
}

Bar::Bar(Ogre::Real max)
{
  this->max = max;
  current = max;
}

Bar::Bar(Ogre::Real max, Ogre::Real current)
{
  this->max = max;
  this->current = current;
}

Bar::~Bar(void)
{
}

bool Bar::isEmpty()
{
  return (current <= 0);
}

bool Bar::isFull()
{
  return (current >= max);
}

Ogre::Real Bar::getFraction()
{
  return (current/max);
}

Ogre::Real Bar::getCurrent()
{
  return current;
}

Ogre::Real Bar::getMax()
{
  return max;
}

void Bar::setMax(Ogre::Real max)
{
  this->max = max;
}

void Bar::setCurrent(Ogre::Real current)
{
  this->current = current;
}

void Bar::setEmpty()
{
  current = 0;
}

void Bar::setFull()
{
  current = max;
}