#pragma once

#include "fmod.hpp"
#include "fmod.h"

#include "NodeComponent.h"
#include "SoundManager.h"
#include "NHException.h"

class SoundManager;

/************************************************************************/
/* Sound Emitter Component.                                             */
/************************************************************************/
class SoundComponent : public NodeComponent
{
public:
  SoundComponent(SoundManager* manager);
  virtual ~SoundComponent(void);

  void update(double elapsedSeconds);

  void play();
  void playLoop();
protected:
  SoundManager* manager;

  FMOD::Channel* channel;
  FMOD::Sound* sound;
  bool loop;
  int loopCount;//-1 for infinite
  float speed;
  float minDistance;
  float maxDistance;

  void hasNodeChange();
};

