#pragma once

#include <OgreMath.h>

class Bar
{
public:
  Bar(Ogre::Real maxValue = 0);
  ~Bar(void);

  Ogre::Real max;
  Ogre::Real current;
};

