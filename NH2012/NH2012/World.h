#pragma once

#include <OgreFrameListener.h>

struct WorldData
{
  int timestamp;
};

class World
{
public:
  World(void);
  ~World(void);

  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations

  WorldData getWorldState();
private:
  WorldData data;
};

