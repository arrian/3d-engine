#pragma once

#include <OgreQuaternion.h>
#include <OgreVector3.h>

class PhysicalInterface
{
public:
  PhysicalInterface(void);
  virtual ~PhysicalInterface(void);

  virtual Ogre::Vector3 getPosition() = 0;
  virtual Ogre::Quaternion getRotation() = 0;

  virtual void setPosition(Ogre::Vector3 position) = 0;
  virtual void setRotation(Ogre::Quaternion rotation) = 0;

  virtual int getID() = 0;
  virtual std::string getType() = 0;//mainly for debug purposes
};

