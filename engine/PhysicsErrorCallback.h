#pragma once

#include "foundation/PxErrorCallback.h"
#include "PxPhysX.h"

class PhysicsErrorCallback : public physx::PxErrorCallback
{
public:
  PhysicsErrorCallback(void);
  ~PhysicsErrorCallback(void);

  virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line);
};

