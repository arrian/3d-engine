#pragma once

#include "fmod.hpp"
#include "fmod.h"

#include "NodeComponent.h"

/************************************************************************/
/* Sound Emitter Component. Must be added to the sound manager to play. */
/************************************************************************/
class SoundComponent : public NodeComponent
{
public:
  SoundComponent(void);
  virtual ~SoundComponent(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);
protected:
  //all these settings will be used by the sound manager when this component is added
  FMOD::Channel* channel;
  FMOD::Sound* sound;
  bool loop;
  int loopCount;//-1 for infinite
  float speed;
  float minDistance;
  float maxDistance;

  void hasNodeChange();
};

