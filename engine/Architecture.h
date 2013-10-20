#pragma once

#include "DataManager.h"
#include "Vector3.h"
#include "Quaternion.h"

class Architecture
{
public:
  ArchitectureDesc description;
  Vector3 position;
  Quaternion quaternion;
  Vector3 scale;

  Architecture(ArchitectureDesc description, Vector3 position = Vector3(0,0,0), Quaternion quaternion = Quaternion::IDENTITY, Vector3 scale = Vector3::UNIT_SCALE)
    : description(description),
      position(position),
      quaternion(quaternion),
      scale(scale)
  {
  }


};