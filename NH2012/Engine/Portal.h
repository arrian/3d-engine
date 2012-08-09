#pragma once

#include <OgreVector3.h>

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
private:
  World* world;
  int id;
  int targetSceneID;//defines the ID of the target scene
  int targetPortalID;
  Ogre::Vector3 position;//defines the portal's target position in the target scene
  Ogre::Vector3 lookAt;
};

