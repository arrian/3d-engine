#pragma once

#include <OgreString.h>

class Item
{
public:
  Item(void);
  virtual ~Item(void);

private:
  Ogre::String name;

  int value;
};

