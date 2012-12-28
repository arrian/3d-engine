#pragma once

#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreRenderWindow.h>
#include <OgreCamera.h>

#include "NodeComponent.h"
#include "PFXSSAO.h"

class CameraComponent : public NodeComponent
{
public:
  CameraComponent(bool enableSSAO, bool enableBloom, bool enableMotionBlur);
  ~CameraComponent(void);

  void hookWindow(Ogre::RenderWindow* window);
  void unhookWindow();
  void rehookWindow();

  void update(double elapsedSeconds);

  Ogre::Vector3 getDirection();

  Ogre::Camera* getCamera();
  Ogre::Viewport* getViewport();
  Ogre::RenderWindow* getWindow();

protected:
  Ogre::Camera* camera;

  Ogre::RenderWindow* window;
  Ogre::Viewport* viewport;

  bool enableBloom;
  bool enableMotionBlur;
  bool enableSSAO;
  PFXSSAO* ssao;

  /*! Player Camera.*/
  int oldCameraWidth;
  int oldCameraHeight;

  void hasNodeChange();

  float nearClipDefault;
  float farClipDefault;
  float rayCastDistance;
};

