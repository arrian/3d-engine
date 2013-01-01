#pragma once
#include "MovementComponent.h"


class DynamicMovementComponent : public MovementComponent
{
public:
  DynamicMovementComponent(Vector3 gravity);
  virtual ~DynamicMovementComponent(void);

  void update(double elapsedSeconds);

protected:


};

