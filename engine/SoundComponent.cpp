#include "SoundComponent.h"


SoundComponent::SoundComponent(SoundManager* manager)
  : manager(manager)
{
}

//-------------------------------------------------------------------------------------
SoundComponent::~SoundComponent(void)
{
}

//-------------------------------------------------------------------------------------
void SoundComponent::update(double elapsedSeconds)
{
  if(channel && node)
  {
    Vector3 position = node->getPosition();
    FMOD_VECTOR pos = {position.x, position.y, position.z};
    FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f};//could implement later
    channel->set3DAttributes(&pos, &vel);
  }
}

//-------------------------------------------------------------------------------------
void SoundComponent::hasNodeChange()
{

}

//-------------------------------------------------------------------------------------
void SoundComponent::play()
{
  manager->playSound(sound, channel);
}

//-------------------------------------------------------------------------------------
void SoundComponent::playLoop()
{
  throw NHException("playLoop in SoundComponent not yet implemented.");
}