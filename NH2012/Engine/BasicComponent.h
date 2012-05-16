#pragma once

#include <OgreFrameListener.h>

class Scene;

class BasicComponent
{
public:
  BasicComponent(void);
  virtual ~BasicComponent(void);

  virtual void frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;
    
  void setScene(Scene* scene);

protected:
  Scene* scene;
  Scene* oldScene;

  virtual void hasSceneChange() = 0;
};
