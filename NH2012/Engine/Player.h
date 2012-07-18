#pragma once

#include <Ogre.h>
#include <limits>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "Bar.h"
#include "Inventory.h"
#include "ComponentList.h"
#include "HumanoidSkeletonComponent.h"
#include "CameraComponent.h"
#include "VisualComponent.h"
#include "Attributes.h"

class Scene;
class World;

class Player
{
public:
  Player(World* world);
  ~Player(void);

  void setScene(Scene* scene, Ogre::Vector3 position, Ogre::Vector3 lookAt);
  void setPosition(Ogre::Vector3 position);
  Ogre::Vector3 getPosition();

  void frameRenderingQueued(const Ogre::FrameEvent& evt);
  void injectKeyDown(const OIS::KeyEvent &evt);
  void injectKeyUp(const OIS::KeyEvent &evt);
  void injectMouseMove(const OIS::MouseEvent &evt);
  void injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

  /*! Hooks the player camera to a render window.*/
  void hook(Ogre::RenderWindow* window);

  void stop();

  void setGravity(float gravity);

private:
  World* world;
  Scene* scene;

  Ogre::SceneNode* node;

  CameraComponent camera;
  HumanoidSkeletonComponent skeleton;
  VisualComponent visual;

  Inventory inventory;

  bool addItem;
};

