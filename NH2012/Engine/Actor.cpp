#include "Actor.h"


Actor::Actor(void* instancePointer, std::string name, Group group)
  : Identifiable(instancePointer, name, group),
    node(NULL),
    position(Vector3::ZERO)
{
}


Actor::~Actor(void)
{
}
