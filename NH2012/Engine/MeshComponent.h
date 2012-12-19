#pragma once

#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include "NodeComponent.h"

class MeshComponent : public NodeComponent
{
public:
  MeshComponent(Ogre::String mesh);
  ~MeshComponent(void);

  void update(double elapsedSeconds);

  Ogre::Entity* getEntity();

protected:
  void hasNodeChange();

private:
  Ogre::String mesh;
  Ogre::Entity* entity;
};

