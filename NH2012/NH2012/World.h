#pragma once

#include <OgreFrameListener.h>

class World
{
public:
  World(void);
  ~World(void);

  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations
};

