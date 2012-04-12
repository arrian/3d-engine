#pragma once

#include "OgreString.h"

namespace Generator
{
  enum NameType
  {
    FANTASY_TOWN,
    FANTASY_MALE,
    FANTASY_FEMALE,
    MODERN_TOWN,
    MODERN_MALE,
    MODERN_FEMALE
  };

  class Name
  {
  public:
    Name(NameType type);
    Name(NameType type, long seed);
    ~Name(void);

    /**
     * Generates a new name.
     */
    Ogre::String generate();

  private:
    NameType type;
  };
}
