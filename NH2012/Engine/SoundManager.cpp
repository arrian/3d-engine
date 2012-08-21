#include "SoundManager.h"


SoundManager::SoundManager(void)
  : sounds2D(),
    sounds3D(),
    channels(),
    system(0),
    music(0)
{
  if(FMOD::System_Create(&system) != FMOD_OK)
  {
    std::cout << "Could not create the sound manager." << std::endl;
  }

  if(system->init(100, FMOD_INIT_NORMAL, 0) != FMOD_OK)
  {
    std::cout << "Could not initialise the sound manager." << std::endl;
  }
 

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
  int result;
  for(std::vector<FMOD::Sound*>::iterator it = sounds2D.begin(); it < sounds2D.end(); ++it)
  {
    (*it)->release();
  }

  //do channels need to be released??

  system->close();
  system->release();
}


//-------------------------------------------------------------------------------------
void SoundManager::playSound(int id)
{

}

//-------------------------------------------------------------------------------------
void SoundManager::playMusic(int id)
{

}

//-------------------------------------------------------------------------------------
void SoundManager::addSoundEmitter(SoundComponent* component)
{
  sounds3D.push_back(component);
  //initialise the sound here
}


