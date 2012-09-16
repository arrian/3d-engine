#pragma once

#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include "NodeComponent.h"

class VisualComponent : public NodeComponent
{
public:
  VisualComponent(Ogre::String mesh);
  ~VisualComponent(void);

  void update(double elapsedSeconds);
  //void frameRenderingQueued(const Ogre::FrameEvent& evt);

protected:
  void hasNodeChange();

private:
  Ogre::String mesh;
  Ogre::Entity* entity;
};

