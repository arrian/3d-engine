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
  PhysicalInterface(int id, std::string name, std::string type);
  virtual ~PhysicalInterface(void);

  int getID();
  std::string getName();
  std::string getType();

protected:
  int id;
  std::string name;
  std::string type;
};

