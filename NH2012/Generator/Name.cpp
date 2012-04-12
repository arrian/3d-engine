#include "Name.h"

namespace Generator
{
  Name::Name(NameType type)
    : type(type)
  {

  }

  Name::Name(NameType type, long seed)
    : type(type)
  {

  }

  Name::~Name(void)
  {
  }

  Ogre::String Name::generate()
  {
    return Ogre::String("null");
  }
}