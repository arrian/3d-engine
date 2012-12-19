#include "Door.h"


Door::Door(void)
  : open(false),
    locked(false)
{
}


Door::~Door(void)
{
}

void Door::setOpen(bool open)
{
  this->open = open;
}

void Door::setLocked(bool locked)
{
  this->locked = locked;
}

bool Door::isOpen()
{
  return open;
}

bool Door::isLocked()
{
  return locked;
}


