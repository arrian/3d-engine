#include "PhysicsErrorCallback.h"

#include <iostream>
#include <sstream>

#include "NHException.h"


//-------------------------------------------------------------------------------------
PhysicsErrorCallback::PhysicsErrorCallback(void)
{
}

//-------------------------------------------------------------------------------------
PhysicsErrorCallback::~PhysicsErrorCallback(void)
{
}


//-------------------------------------------------------------------------------------
void PhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
  std::stringstream ss;
  ss << "physics error " << code << ": " << message << " From " << file << " on line " << line << std::endl;

  throw NHException(ss.str().c_str());
}

