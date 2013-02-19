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

  //Getters
  Vector3 getDirection();
  Ogre::Camera* getCamera();
  Ogre::Viewport* getViewport();
  Ogre::RenderWindow* getWindow();

  //Setters
  void setBloomEnabled(bool enabled);
  void setSSAOEnabled(bool enabled);
  void setShadowsEnabled(bool enabled);
  /*
  void setHDREnabled(bool enabled);
  void setShadowsEnabled(bool enabled);
  void setLightingEnabled(bool enabled);
  void setEffectsEnabled(bool enabled);
  void setDecalsEnabled(bool enabled);
  void setWaterEnabled(bool enabled);
  void setSkyEnabled(bool enabled);
  void setFreeCameraEnabled(bool enabled);
  void setWireframeEnabled(bool enabled);
  */

  //Assertions
  bool isShadowsEnabled() {return enableShadows;}
  bool isBloomEnabled() {return enableBloom;}
  bool isSSAOEnabled() {return enableSSAO;}
  /*
  bool isHDREnabled();
  bool isLightingEnabled();
  bool isEffectsEnabled();
  bool isDecalsEnabled();
  bool isWaterEnabled();
  bool isSkyEnabled();
  bool isFreeCameraEnabled();
  bool isWireframeEnabled();
  */

protected:
  Ogre::Camera* camera;

  Ogre::RenderWindow* window;
  Ogre::Viewport* viewport;

  //Flags
  bool enableShadows;
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

