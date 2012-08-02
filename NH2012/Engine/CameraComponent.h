#pragma once

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>

#include "NodeComponent.h"

class CameraComponent : public NodeComponent
{
public:
  CameraComponent();
  ~CameraComponent(void);

  void hookWindow(Ogre::RenderWindow* window);
  void unhookWindow();

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void rayQuery();

private:
  Ogre::Camera* camera;

  Ogre::RenderWindow* window;
  Ogre::Viewport* viewport;

  /*! Player Camera.*/
  int oldCameraWidth;
  int oldCameraHeight;
  //Ogre::SceneNode* cameraNode;

  void hasNodeChange();
};

