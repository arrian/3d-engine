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
  PhysicalInterface(std::string name, std::string type);
  virtual ~PhysicalInterface(void);

  int getInstanceID();
  std::string getName();
  std::string getType();

  void setInstanceID(int instanceID);

  //virtual void setPosition(Ogre::Vector3 position) = 0;
  //virtual Ogre::Vector3 getPosition() = 0;
  //maybe add more getters and setters here. eg. velocity and facing

protected:
  int instanceID;
  std::string name;
  std::string type;
};

