#include "PhysicalInterface.h"

#include "NHException.h"

//-------------------------------------------------------------------------------------
PhysicalInterface::PhysicalInterface(std::string name, std::string type)
  : instanceID(-1),
    name(name),
    type(type)
{
}

//-------------------------------------------------------------------------------------
PhysicalInterface::~PhysicalInterface(void)
{
}

//-------------------------------------------------------------------------------------
int PhysicalInterface::getInstanceID()
{
  return instanceID;
}

//-------------------------------------------------------------------------------------
void PhysicalInterface::setInstanceID(int instanceID)
{
  this->instanceID = instanceID;
}

//-------------------------------------------------------------------------------------
std::string PhysicalInterface::getType()
{
  return type;
}

//-------------------------------------------------------------------------------------
std::string PhysicalInterface::getName()
{
  return name;
}

