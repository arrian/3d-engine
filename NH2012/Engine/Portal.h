#pragma once

#include "Vector3.h"
#include <OgreAxisAlignedBox.h>

#include "World.h"
#include "Scene.h"

#include "Id.h"

/** 
 * A portal to transport the player or creatures.
 */
class Portal
{
public:
  Portal(Id<Scene> targetSceneID, Id<Portal> targetPortalID, Vector3 position, Vector3 lookAt);
  ~Portal(void);

  Id<Scene> getTargetScene();
  Id<Portal> getTargetPortal();

  Vector3 getPosition();
  Vector3 getLookAt();

  bool isLoadRequired(Vector3 observerPosition);
private:
  Id<Scene> targetSceneId;
  Id<Portal> targetPortalId;
  
  Vector3 position;//defines the portal's target position in the target scene
  Vector3 lookAt;

  Ogre::AxisAlignedBox loadArea;//area in which the target scene should be loaded
  Ogre::AxisAlignedBox moveArea;//area in which the observer should be moved to the target scene
};

