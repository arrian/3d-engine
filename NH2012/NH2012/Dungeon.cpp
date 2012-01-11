#include "Dungeon.h"


Dungeon::Dungeon(void)
{
}

Dungeon::Dungeon(Ogre::String name, DungeonType::Type type)
{
  this->name = name;
  this->type = type;

  if(type == DungeonType::PREDEFINED)
  {
    //load level from file
  }
  else if(type == DungeonType::CAVE)
  {
    //generate perlin noise cave
  }
  else if(type == DungeonType::OVERWORLD)
  {
    //generate perlin noise landscape
  }
  else if(type == DungeonType::DUNGEON)
  {
    //generate nethack style dungeon
  }
  else
  {
    //generate other...
  }
}



Dungeon::~Dungeon(void)
{
}
