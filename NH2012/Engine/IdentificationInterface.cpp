#include "IdentificationInterface.h"

#include "NHException.h"

//-------------------------------------------------------------------------------------
IdentificationInterface::IdentificationInterface(void* instancePointer, std::string name, Group group)
  : instancePointer(instancePointer),
    instanceID(-1),
    name(name),
    group(group)
{
}

//-------------------------------------------------------------------------------------
IdentificationInterface::~IdentificationInterface(void)
{
}

//-------------------------------------------------------------------------------------
int IdentificationInterface::getInstanceID()
{
  return instanceID;
}

//-------------------------------------------------------------------------------------
void IdentificationInterface::setInstanceID(int instanceID)
{
  this->instanceID = instanceID;
}

//-------------------------------------------------------------------------------------
Group IdentificationInterface::getGroup()
{
  return group;
}

//-------------------------------------------------------------------------------------
bool IdentificationInterface::isInGroup(Group group)
{
  return (group & this->group) == group;//is this correct?
}

//-------------------------------------------------------------------------------------
std::string IdentificationInterface::getName()
{
  return name;
}

//-------------------------------------------------------------------------------------
void* IdentificationInterface::getInstancePointer()
{
  return instancePointer;
}



