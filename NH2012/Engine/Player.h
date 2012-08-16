#pragma once

#include <Ogre.h>
#include <limits>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "Bar.h"
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

  void frameRenderingQueued(const Ogre::FrameEvent& evt);
  void hook(Ogre::RenderWindow* window);/*! Hooks the player camera to a render window.*/
  void stop();

  //Setters
  void setScene(Scene* scene, Ogre::Vector3 position, Ogre::Vector3 lookAt);
  void setPosition(Ogre::Vector3 position);
  void setGravity(float gravity);
  void setCollisionEnabled(bool enabled);

  //Getters
  Ogre::Vector3 getPosition();

  //Injectors
  void injectKeyDown(const OIS::KeyEvent &evt);
  void injectKeyUp(const OIS::KeyEvent &evt);
  void injectMouseMove(const OIS::MouseEvent &evt);
  void injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

private:
  World* world;
  Scene* scene;

  Ogre::SceneNode* node;//world location of the player

  CameraComponent camera;//the player camera generally attached to the head node
  HumanoidSkeletonComponent skeleton;
  VisualComponent visual;

  bool addItem;

  float placementDistance;
  float lookResponsiveness;
};

