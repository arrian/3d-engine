#include "Identifiable.h"

#include "NHException.h"

//-------------------------------------------------------------------------------------
Identifiable::Identifiable(void* instancePointer, std::string name, Group group)
  : instancePointer(instancePointer),
    instanceID(-1),
    name(name),
    group(group)
{
}

//-------------------------------------------------------------------------------------
Identifiable::~Identifiable(void)
{
}

//-------------------------------------------------------------------------------------
int Identifiable::getInstanceID()
{
  return instanceID;
}

//-------------------------------------------------------------------------------------
void Identifiable::setInstanceID(int instanceID)
{
  this->instanceID = instanceID;
}

//-------------------------------------------------------------------------------------
Group Identifiable::getGroup()
{
  return group;
}

//-------------------------------------------------------------------------------------
bool Identifiable::isInGroup(Group group)
{
  return (group & this->group) == group;//is this correct?
}

//-------------------------------------------------------------------------------------
std::string Identifiable::getName()
{
  return name;
}

//-------------------------------------------------------------------------------------
void* Identifiable::getInstancePointer()
{
  return instancePointer;
}



