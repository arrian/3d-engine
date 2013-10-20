#pragma once

//Ogre
#include <OgreRoot.h>

#include "CameraComponent.h"

#include "PFXSSAO.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class GraphicsManager
{
public:
  GraphicsManager(void);
  virtual ~GraphicsManager(void);

  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player

  Ogre::SceneManager* createSceneGraphicsManager() 
  {
    try
    {
      boost::shared_ptr<Ogre::Root> root_ptr = root.lock();
      return root_ptr->createSceneManager(Ogre::ST_GENERIC);
    }
    catch(boost::bad_weak_ptr b)
    {
#ifdef _DEBUG
      std::cout << "Failed to create scene graphics manager. Root has expired.";
#endif
    }

    return NULL;
  }

  void destroySceneGraphicsManager(Ogre::SceneManager* sceneGraphicsManager) 
  {
    if(!sceneGraphicsManager) return;
    try
    {
      boost::shared_ptr<Ogre::Root> root_ptr = root.lock();
      root_ptr->destroySceneManager(sceneGraphicsManager);
    }
    catch(boost::bad_weak_ptr b)
    {
#ifdef _DEBUG
      std::cout << "Failed to destroy scene graphics manager. Root has expired.";
#endif      
    }
  }
  
  //Getters
  boost::shared_ptr<Ogre::Root> getRoot() 
  {
    try
    {
      return boost::shared_ptr<Ogre::Root>(root.lock());
    }
    catch(boost::bad_weak_ptr b)
    {
#ifdef _DEBUG
      std::cout << "Could not get root from graphics manager. Root has expired." << std::endl;
#endif
    }
    return boost::shared_ptr<Ogre::Root>();
  }

  CameraComponent* getCamera() {return camera;}
  Ogre::RenderWindow* getWindow() {return camera->getWindow();}

  //Setters
  void setRoot(boost::shared_ptr<Ogre::Root> root);
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
  boost::weak_ptr<Ogre::Root> root;
  CameraComponent* camera;
  Ogre::RenderWindow* window;

  PFXSSAO* ssao;

  //Flags
  bool enableShadows;
  bool enableBloom;
  bool enableSSAO;

  void refresh();
};

