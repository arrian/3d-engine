#pragma once
#include <OgreString.h>

namespace Packet
{
  struct World
  {
    unsigned int timestamp;

    template <typename Archive>
    Ogre::String serialise(Archive& ar, const unsigned int version)
    {
      ar & timestamp;


    }

  };

  struct Player
  {
    unsigned int timestamp;

    Ogre::Vector3 playerPosition();

    template <typename Archive>
    Ogre::String serialise(Archive& ar, const unsigned int version)
    {
      ar & timestamp;


    }
  };
}