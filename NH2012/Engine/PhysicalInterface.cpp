#include "PhysicalInterface.h"

#include "NHException.h"

//-------------------------------------------------------------------------------------
PhysicalInterface::PhysicalInterface(int id, std::string name, std::string type)
  : id(id),
    name(name),
    type(type)
{
}

//-------------------------------------------------------------------------------------
PhysicalInterface::~PhysicalInterface(void)
{
}

//-------------------------------------------------------------------------------------
int PhysicalInterface::getID()
{
  return id;
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

