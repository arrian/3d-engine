#pragma once

#include <vector>

#include <OgreVector3.h>

#include "Dungeon.h"

class AI
{
public:
  AI(void);
  ~AI(void);
  
  std::vector<Ogre::Vector3> pathfind(Dungeon* dungeon, Ogre::Vector3 from, Ogre::Vector3 to);
  
};

