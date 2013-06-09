#pragma once

#include "Vector3.h"
#include <OgreAxisAlignedBox.h>

#include "World.h"
#include "Scene.h"

/** 
 * A portal to transport the player or creatures.
 */
class Portal
{
public:
  Portal(int id, int targetSceneID, int targetPortalID, Vector3 position, Vector3 lookAt);
  ~Portal(void);

  int getTargetScene();//gets the target scene for this portal
  int getTargetPortal();
  int getID();

  Vector3 getPosition();
  Vector3 getLookAt();

  bool isLoadRequired(Vector3 observerPosition);
private:
  int id;
  int targetSceneID;//defines the ID of the target scene
  int targetPortalID;
  Vector3 position;//defines the portal's target position in the target scene
  Vector3 lookAt;

  Ogre::AxisAlignedBox loadArea;//area in which the target scene should be loaded
  Ogre::AxisAlignedBox moveArea;//area in which the observer should be moved to the target scene
};

