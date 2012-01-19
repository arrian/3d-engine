#pragma once

#include <OgreString.h>

#include "Attributes.h"

class Item
{
public:
  Item(void);
  virtual ~Item(void);

  friend bool operator==(const Item& x, const Item& y);
private:
  int id;//unique identifier
  Ogre::String name;

  int value;

  ItemAttributes attributes;
};

