#pragma once

#include "Vector3.h"
#include "Quaternion.h"
#include <OgreRenderWindow.h>
#include <OgreCamera.h>

#include "NodeComponent.h"
#include "PFXSSAO.h"

class CameraComponent : public NodeComponent
{
public:
  CameraComponent(bool enableSSAO, bool enableBloom);
  ~CameraComponent(void);

  void hookWindow(Ogre::RenderWindow* window);
  void unhookWindow();
  void rehookWindow();

  void update(double elapsedSeconds);

  Vector3 getDirection();

  Ogre::Camera* getCamera();
  Ogre::Viewport* getViewport();
  Ogre::RenderWindow* getWindow();

protected:
  Ogre::Camera* camera;

  Ogre::RenderWindow* window;
  Ogre::Viewport* viewport;

  bool enableBloom;
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

