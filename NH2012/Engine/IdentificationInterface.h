#pragma once

#include <OgreQuaternion.h>
#include <OgreVector3.h>

/**
 * A generic physical interface.
 * Will be used for scripting and the console as a generic interface to anything that exists in the world.
 */
class IdentificationInterface
{
public:
  IdentificationInterface(std::string name, std::string type);
  virtual ~IdentificationInterface(void);

  int getInstanceID();
  std::string getName();
  std::string getType();

  void setInstanceID(int instanceID);

protected:
  int instanceID;
  std::string name;
  std::string type;
};
