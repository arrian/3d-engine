#pragma once

#include <OgreString.h>

namespace DungeonType
{
  enum Type
  {
    OVERWORLD,
    UNDERWORLD,
    CAVE,
    DUNGEON,
    TOWN,
    PREDEFINED,
    ASTRAL
  };
}

class Dungeon
{
public:
  Dungeon(void);
  Dungeon(Ogre::String name, DungeonType::Type type);
  ~Dungeon(void);

  Ogre::String name;
  DungeonType::Type type;
};

