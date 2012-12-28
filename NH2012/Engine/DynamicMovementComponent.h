#pragma once
#include "NodeComponent.h"


class DynamicMovementComponent : public NodeComponent
{
public:
  DynamicMovementComponent(Ogre::Vector3 gravity);
  virtual ~DynamicMovementComponent(void);

protected:
  Ogre::Vector3 gravity;
};

