#pragma once


#include "staticinterface.h"
#include <OgreVector3.h>
#include <OgreQuaternion.h>

class DynamicInterface : public StaticInterface
{
public:
  virtual ~DynamicInterface(void) {}

  virtual Ogre::Vector3 getPosition() = 0;
  virtual Ogre::Quaternion getRotation() = 0;
  virtual Ogre::Vector3 getVelocity() = 0;

  virtual void setPosition(Ogre::Vector3 position) = 0;
  virtual void setRotation(Ogre::Quaternion rotation) = 0;
  virtual void setVelocity(Ogre::Vector3 velocity) = 0;
};

