#include "Bar.h"

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
