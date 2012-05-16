#include "PhysicsErrorCallback.h"

#include <iostream>

PhysicsErrorCallback::PhysicsErrorCallback(void)
{
}


PhysicsErrorCallback::~PhysicsErrorCallback(void)
{
}

void PhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
  std::cout << "Physx error: " << message << std::endl;
}

