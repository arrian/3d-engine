#pragma once

#include <OgreVector3.h>
#include <OgreAxisAlignedBox.h>

#include "World.h"
#include "Scene.h"

/** 
 * A portal to transport the player or monsters.
 */
class Portal
{
public:
  Portal(int id, int targetSceneID, int targetPortalID, Ogre::Vector3 position, Ogre::Vector3 lookAt);
  ~Portal(void);

  int getTargetScene();//gets the target scene for this portal
  int getTargetPortal();
  int getID();

  Ogre::Vector3 getPosition();
  Ogre::Vector3 getLookAt();

  bool isLoadRequired(Ogre::Vector3 observerPosition);
private:
  int id;
  int targetSceneID;//defines the ID of the target scene
  int targetPortalID;
  Ogre::Vector3 position;//defines the portal's target position in the target scene
  Ogre::Vector3 lookAt;

  Ogre::AxisAlignedBox loadArea;//area in which the target scene should be loaded
  Ogre::AxisAlignedBox moveArea;//area in which the observer should be moved to the target scene
};

