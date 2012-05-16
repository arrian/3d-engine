#pragma once

#include <OgreVector3.h>
#include "VisualComponent.h"
#include "Scene.h"

class Portal
{
public:
  Portal(Scene* scene, int targetID, Ogre::Vector3 position);
  ~Portal(void);

  Scene* getTarget();

private:

  VisualComponent* visual;

  //Ogre::AxisAlignedBox* loadArea;//area in which to load the target scene
  Scene* scene;
  Scene* target;
  int targetID;

  bool requiresActivation;

  void loadTarget();
};

