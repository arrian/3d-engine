#pragma once

#include <Vector3.h> 
#include <Quaternion.h>

class Physical
{

public:
  Physical() {}
  virtual ~Physical() {}

  virtual void setPosition(Vector3 position) = 0;
  virtual Vector3 getPosition() = 0;

  virtual void setRotation(Quaternion rotation) = 0;
  virtual Quaternion getRotation() = 0;

};