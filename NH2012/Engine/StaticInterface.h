#pragma once

#include <OgreVector3.h>
#include <OgreQuaternion.h>

class StaticInterface
{
public:
  virtual ~StaticInterface(void) {}

  virtual Ogre::Vector3 getPosition() = 0;
  virtual Ogre::Quaternion getRotation() = 0;
};

