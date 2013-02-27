#pragma once

//Ogre
#include <OgreRoot.h>

#include "CameraComponent.h"

#include "PFXSSAO.h"

class GraphicsManager
{
public:
  GraphicsManager(void);
  virtual ~GraphicsManager(void);

  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player

  Ogre::SceneManager* createSceneGraphicsManager() {if(root) return root->createSceneManager(Ogre::ST_GENERIC); return NULL;}
  void destroySceneGraphicsManager(Ogre::SceneManager* sceneGraphicsManager) {if(root && sceneGraphicsManager) root->destroySceneManager(sceneGraphicsManager);}
  
  //Getters
  Ogre::Root* getRoot() {return root;}
  CameraComponent* getCamera() {return camera;}
  Ogre::RenderWindow* getWindow() {return camera->getWindow();}

  //Setters
  void setRoot(Ogre::Root* root);
  void setCamera(CameraComponent* camera);

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

private:
  Ogre::Root* root;
  CameraComponent* camera;
  Ogre::RenderWindow* window;

  PFXSSAO* ssao;

  //Flags
  bool enableShadows;
  bool enableBloom;
  bool enableSSAO;

  void refresh();
};

