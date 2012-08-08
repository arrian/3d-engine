#pragma once

#include <OgreQuaternion.h>
#include <OgreVector3.h>

/**
 * A generic physical interface.
 * Will be used for scripting and the console as a generic interface to anything that exists in the world.
 */
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

