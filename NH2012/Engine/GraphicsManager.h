#pragma once

//Ogre
#include <OgreRoot.h>

#include "CameraComponent.h"

class GraphicsManager
{
public:
  GraphicsManager(void);
  virtual ~GraphicsManager(void);

  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player

  Ogre::SceneManager* createSceneGraphicsManager() {return root->createSceneManager(Ogre::ST_GENERIC);}
  void destroySceneGraphicsManager(Ogre::SceneManager* sceneGraphicsManager) {root->destroySceneManager(sceneGraphicsManager);}
  
  //Getters
  Ogre::Root* getRoot() {return root;}
  CameraComponent* getCamera() {return camera;}

  //Setters
  void setRoot(Ogre::Root* root);
  void setCamera(CameraComponent* camera);

private:
  Ogre::Root* root;
  CameraComponent* camera;
};

