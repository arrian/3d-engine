#include "SoundComponent.h"


SoundComponent::SoundComponent(void)
{
}


SoundComponent::~SoundComponent(void)
{
}

void SoundComponent::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(channel)
  {
    Ogre::Vector3 position = node->getPosition();
    FMOD_VECTOR pos = {position.x, position.y, position.z};
    FMOD_VECTOR vel = {0.0f, 0.0f, 0.0f};//could implement later
    channel->set3DAttributes(&pos, &vel);
  }
}

void SoundComponent::hasNodeChange()
{

}