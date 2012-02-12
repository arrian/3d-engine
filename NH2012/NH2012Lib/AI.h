#pragma once

#include <vector>

#include <OgreVector3.h>

#include "Dungeon.h"

class AI
{
public:
  AI(void);
  ~AI(void);
  
  /*! Finds a path from one point in the dungeon to another point in the dungeon.*/
  std::vector<Ogre::Vector3> pathfind(Dungeon* dungeon, Ogre::Vector3 from, Ogre::Vector3 to);
  
};

