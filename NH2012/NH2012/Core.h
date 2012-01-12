#pragma once

#include <OgreFrameListener.h>

class Core
{
public:
  Core(void);
  ~Core(void);

  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations
};

