#pragma once

#include <vector>
#include <iostream>

#include "fmod.hpp"
#include "fmod.h"

#include "SoundComponent.h"

/************************************************************************/
/* Sound wrapper using FMOD                                             */
/************************************************************************/
class SoundManager
{
public:
  SoundManager(void);
  ~SoundManager(void);

  void addSoundEmitter(SoundComponent* component);//adds a sound attached to a node for 3d sound
  void playSound(int id);//play a 2d sound now
  void playMusic(int id);//can only play one music track at a time

private:
  FMOD::System* system;
  FMOD::Sound* music;
  std::vector<FMOD::Sound*> sounds2D;
  std::vector<SoundComponent*> sounds3D;
  std::vector<FMOD::Channel*> channels;
};

