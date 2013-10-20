#pragma once

#include "Quaternion.h"
#include "Vector3.h"

#include "Group.h"

/**
 * A generic physical interface.
 * Will be used for scripting and the console as a generic interface to anything that exists in the world.
 */
class Identifiable
{
public:
  Identifiable(void* instancePointer, std::string name, Group group);
  virtual ~Identifiable(void);


  template<class T> T* getInstancePointer() {return static_cast<T*>(instancePointer);}


  int getInstanceID();
  std::string getName();
  Group getGroup();

  void setInstanceID(int instanceID);

  bool isInGroup(Group group);

protected:
  void* instancePointer;
  int instanceID;
  std::string name;
  Group group;
};

