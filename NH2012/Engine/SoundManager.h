#pragma once

#include <vector>

#include "fmod.hpp"
#include "fmod.h"

/************************************************************************/
/* Sound wrapper using FMOD                                             */
/************************************************************************/
class SoundManager
{
public:
  SoundManager(void);
  ~SoundManager(void);

  FMOD::System* system;
  std::vector<FMOD::Sound*> sounds;
  std::vector<FMOD::Channel*> channels;
};

