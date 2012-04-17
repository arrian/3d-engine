#pragma once

#include <vector>

#include <OgreVector3.h>

#include "Cell.h"

class AI
{
public:
  AI(void);
  ~AI(void);
  
  /*! Finds a path from one point in the dungeon to another point in the dungeon.*/
  std::vector<Ogre::Vector3> pathfind(Cell* cell, Ogre::Vector3 from, Ogre::Vector3 to);
  
};

