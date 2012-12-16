#pragma once
#include "NodeComponent.h"

#include <PxPhysics.h>
#include "geometry/PxGeometry.h"
#include <OgreVector3.h>

class QueryComponent :
  public NodeComponent
{
public:
  QueryComponent(void);
  virtual ~QueryComponent(void);

  void update(double elapsedSeconds);

  bool rayQuery(Ogre::Vector3 direction, float distance);
  bool sweepQuery(Ogre::Vector3 direction, float distance);
  bool overlapQuery();

protected:
  void hasNodeChange();

  Ogre::ManualObject* debugHit;
  Ogre::Entity* debugHitPosition;
  Ogre::SceneNode* hitNode;
};

