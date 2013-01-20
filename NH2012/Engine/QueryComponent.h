#pragma once

#include <PxPhysics.h>
#include "geometry/PxGeometry.h"
#include "Vector3.h"

#include "NodeComponent.h"
#include "Identifiable.h"
#include "Group.h"


class QueryComponent : public NodeComponent
{
public:
  QueryComponent(void);
  virtual ~QueryComponent(void);

  void update(double elapsedSeconds);

  Identifiable* rayQuery(Vector3 direction, float distance, Group groups);//returns hit or null
  Identifiable* rayQuery(Vector3 from, Vector3 direction, float distance, Group groups);//returns hit or null
  bool sweepQuery(Vector3 direction, float distance);
  bool overlapQuery();

protected:
  void hasNodeChange();
};

