#pragma once

#include <OgreString.h>

class Entity
{
public:
  Entity(int id = 0, Ogre::String name = Ogre::String("DefaultEntity"));
  ~Entity(void);
protected:
  int id;
  Ogre::String name;
};

