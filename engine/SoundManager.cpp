#include "SoundManager.h"

#include <OgreResourceGroupManager.h>
#include "NHException.h"

SoundManager::SoundManager(void)
  : sounds2D(),
    sounds3D(),
    channels(),
    system(NULL),
    music(NULL),
    musicChannel(NULL)
{
  if(FMOD::System_Create(&system) != FMOD_OK) throw NHException("could not create the sound manager");

  if(system->init(100, FMOD_INIT_NORMAL, 0) != FMOD_OK) throw NHException("could not initialise the sound manager");
 

  //Testing sound
  /*FMOD::Sound* sound1;
  FMOD::Channel* channel1 = 0;
  if(system->createSound("./boom.wav", FMOD_2D, 0, &sound1) != FMOD_OK)
  {
    std::cout << "there was an error creating a sound" << std::endl;

  }

  sound1->setMode(FMOD_LOOP_NORMAL);

  if(system->playSound(FMOD_CHANNEL_FREE, sound1, false, &channel1) != FMOD_OK)
  {
    std::cout << "could not play the sound" << std::endl;
  }
  channel1->setPaused(false);
  
  std::cout << "finished initialising the sound manager" << std::endl;
  */
}

//-------------------------------------------------------------------------------------
SoundManager::~SoundManager(void)
{
  //int result;
  for(std::vector<FMOD::Sound*>::iterator it = sounds2D.begin(); it < sounds2D.end(); ++it)
  {
    (*it)->release();
  }

  //do channels need to be released??

  system->close();
  system->release();
}


//-------------------------------------------------------------------------------------
void SoundManager::playSound(FMOD::Sound* sound, FMOD::Channel* channel)
{
  system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
}

//-------------------------------------------------------------------------------------
void SoundManager::playMusic(int id)
{
  throw NHException("not implemented");
  if(music != NULL) music->release();
  music = NULL;
  //get the song path from the id and create a sound
  system->playSound(FMOD_CHANNEL_FREE, music, false, &musicChannel);
}

//-------------------------------------------------------------------------------------
void SoundManager::addSoundEmitter(SoundComponent* component)
{
  sounds3D.push_back(component);
  throw NHException("not implemented");
  //initialise the sound here
}

//-------------------------------------------------------------------------------------
void SoundManager::setMuteAll(bool mute)
{
  musicChannel->setMute(mute);

  for(std::vector<FMOD::Channel*>::iterator iter = channels.begin(); iter < channels.end(); ++iter)
  {
    (*iter)->setMute(mute);
  }
}

//-------------------------------------------------------------------------------------
std::string SoundManager::getSoundPath(std::string filename)
{
  Ogre::FileInfoListPtr fileListPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("Essential", filename);
  if(fileListPtr->size() < 1) throw NHException("could not find the path to the specified sound file");

  return fileListPtr->front().archive->getName() + "/" + fileListPtr->front().filename;
}

//-------------------------------------------------------------------------------------
void SoundManager::setEnabled(bool enabled)
{
  this->enabled = enabled;
  setMuteAll(!enabled);
  //disable updates
}

//-------------------------------------------------------------------------------------
bool SoundManager::isEnabled()
{
  return enabled;
}
