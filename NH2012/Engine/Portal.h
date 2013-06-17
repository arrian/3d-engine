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
  Portal(PortalDesc desc, Id<Scene> targetSceneID, Id<Portal> targetPortalID);
  ~Portal(void);

  Id<Scene> getTargetScene();
  Id<Portal> getTargetPortal();

  Vector3 getPosition();
  Vector3 getLookAt();

  void setPosition(Vector3 position) {this->position = position;}
  void setLookAt(Vector3 lookAt) {this->lookAt = lookAt;}
  void setScene(Scene* scene) {this->scene = scene;}

  void update(double elapsedSeconds);

  bool isLoadRequired(Vector3 observerPosition);
private:
  Scene* scene;
  PortalDesc desc;

  Id<Scene> targetSceneId;
  Id<Portal> targetPortalId;
  
  Vector3 position;//defines the portal's target position in the target scene
  Vector3 lookAt;

  Ogre::AxisAlignedBox loadArea;//area in which the target scene should be loaded
  Ogre::AxisAlignedBox moveArea;//area in which the observer should be moved to the target scene
};

