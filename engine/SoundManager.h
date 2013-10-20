#pragma once

#include <vector>
#include <iostream>

#include "fmod.hpp"
#include "fmod.h"

#include "SoundComponent.h"

class SoundComponent;

/************************************************************************/
/* Sound wrapper using FMOD                                             */
/************************************************************************/
class SoundManager
{
public:
  SoundManager(void);
  virtual ~SoundManager(void);

  void addSoundEmitter(SoundComponent* component);//adds a sound attached to a node for 3d sound
  void playSound(FMOD::Sound* sound, FMOD::Channel* channel);//play a 2d sound
  void playMusic(int id);//can only play one music track at a time

  void setMuteAll(bool mute);

  void setEnabled(bool enabled);
  bool isEnabled();

private:
  FMOD::System* system;
  FMOD::Sound* music;
  FMOD::Channel* musicChannel;
  std::vector<FMOD::Sound*> sounds2D;
  std::vector<SoundComponent*> sounds3D;
  std::vector<FMOD::Channel*> channels;

  std::string getSoundPath(std::string filename);

  bool enabled;
};

