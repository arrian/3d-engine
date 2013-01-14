#pragma once

#include <PxPhysics.h>
#include "geometry/PxGeometry.h"
#include "Vector3.h"

#include "NodeComponent.h"
#include "IdentificationInterface.h"
#include "Group.h"


class QueryComponent : public NodeComponent
{
public:
  QueryComponent(void);
  virtual ~QueryComponent(void);

  void update(double elapsedSeconds);

  IdentificationInterface* rayQuery(Vector3 direction, float distance, Group groups);//returns hit or null
  IdentificationInterface* rayQuery(Vector3 from, Vector3 direction, float distance, Group groups);//returns hit or null
  bool sweepQuery(Vector3 direction, float distance);
  bool overlapQuery();

protected:
  void hasNodeChange();
};

