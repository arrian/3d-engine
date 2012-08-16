#include "SoundManager.h"


SoundManager::SoundManager(void)
  : sounds(),
    channels()
{


}


SoundManager::~SoundManager(void)
{
  int result;
  for(std::vector<FMOD::Sound*>::iterator it = sounds.begin(); it < sounds.end(); ++it)
  {
    (*it)->release();
  }

  //do channels need to be released??

  system->close();
  system->release();
}
