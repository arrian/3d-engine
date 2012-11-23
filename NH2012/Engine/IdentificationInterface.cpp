#include "IdentificationInterface.h"

#include "NHException.h"

//-------------------------------------------------------------------------------------
IdentificationInterface::IdentificationInterface(std::string name, std::string type)
  : instanceID(-1),
    name(name),
    type(type)
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
std::string IdentificationInterface::getType()
{
  return type;
}

//-------------------------------------------------------------------------------------
std::string IdentificationInterface::getName()
{
  return name;
}

