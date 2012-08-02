#pragma once

#include <OgreVector3.h>
#include "VisualComponent.h"
#include "Scene.h"

/** 
 * A portal to transport the player or monsters.
 */
class Portal
{
public:
  Portal(Scene* scene, int targetID, Ogre::Vector3 position, Ogre::Vector3 targetPosition);
  ~Portal(void);

  Scene* getTarget();//gets the target scene for this portal


  void injectObserver(NodeComponent* component);//injects the observer position for target loading

private:
  Item* door;//defines the portal entrance device (eg. door). Zeroed if not required.

  Scene* scene;//defines the scene that this portal is located in
  Scene* target;//defines the target scene for this portal
  int targetID;//defines the ID of the target scene
  Ogre::Vector3 targetPosition;//defines the portal's target position in the target scene

  bool isInternal;//defines if this portal is internal to the current scene (ie. no scene load needed)

  Ogre::Real loadDistance;//defines the distance from the focal point to load the next scene

  bool requiresActivation;//determines if the user needs to interact with the portal first. In general, stick to true if 'Item* door' exists, false otherwise.
  Ogre::Vector3 focalPoint;//defines the location at which a scene change is required for a non-interactive door

  void loadTarget();//loads the target scene
  bool hasLoadedTarget();//returns true if the target scene has been loaded.
};

