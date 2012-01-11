#pragma once

#include <OgreString.h>

/**
 * Stores the global game state
 */
class Flag
{
public:
  Flag(void);
  virtual ~Flag(void);

  bool explore;//explore mode
  bool wizard;//wizard mode

  bool friday13;

  bool verbose;//verbose logging

  Ogre::Real moonPhase;
  Ogre::String serialiseMoonPhase(Ogre::Real moonPhase);

private:
  Ogre::Real calculateMoonPhase(int day, int month, int year);
};

