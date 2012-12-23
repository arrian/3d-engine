#pragma once

#include <PxPhysics.h>
#include "geometry/PxGeometry.h"
#include <OgreVector3.h>

#include "NodeComponent.h"
#include "IdentificationInterface.h"
#include "Group.h"


class QueryComponent :
  public NodeComponent
{
public:
  QueryComponent(void);
  virtual ~QueryComponent(void);

  void update(double elapsedSeconds);

  IdentificationInterface* rayQuery(Ogre::Vector3 direction, float distance, Group groups);//returns hit or null
  bool sweepQuery(Ogre::Vector3 direction, float distance);
  bool overlapQuery();

protected:
  void hasNodeChange();

  //Ogre::Entity* debugHitPosition;
  //Ogre::SceneNode* hitNode;
};

