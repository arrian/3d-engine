#pragma once

#include <OgreString.h>

#include "Attributes.h"

class ItemData
{
public:
  ItemData(void);
  ~ItemData(void);

  friend bool operator==(const ItemData& x, const ItemData& y);
protected:
  int value;
  Ogre::String itemName;
  ItemAttributes attributes;
};

