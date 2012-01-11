#pragma once

#include <OgreString.h>

#include "Attributes.h"

class Item
{
public:
  Item(void);
  virtual ~Item(void);

private:
  Ogre::String name;

  int value;

  ItemAttributes attributes;
};

