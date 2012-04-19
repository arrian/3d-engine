#pragma once

#include <Ogre.h>
#include <limits>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "Cell.h"
#include "Actor.h"
#include "Bar.h"
#include "Inventory.h"
#include "Attributes.h"
#include "Environment.h"

//character physics
#include "Shapes\OgreBulletCollisionsCapsuleShape.h"
#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"

class Cell;

/*! User data.*/
class Player
{
public:
  Player(Environment* environment, Ogre::RenderWindow* window);
  ~Player(void);

  void setCell(Cell* cell, Ogre::Vector3 position, Ogre::Vector3 lookAt);
  Ogre::Vector3 getPosition();

  void frameRenderingQueued(const Ogre::FrameEvent& evt);
  void injectKeyDown(const OIS::KeyEvent &evt);
  void injectKeyUp(const OIS::KeyEvent &evt);
  void injectMouseMove(const OIS::MouseEvent &evt);
  void injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

  /*! Hooks the player camera to a render window.*/
  void hook(Ogre::RenderWindow* window);

private:
  Inventory inventory;
  
  /*! Player Camera.*/
  int oldCameraWidth;
  int oldCameraHeight;
  Ogre::SceneNode* cameraNode;
  Ogre::Camera* camera;
  Ogre::Real playerHeight;//TODO implement
  Ogre::RenderWindow* window;
  Ogre::Viewport* vp;

  /* Player Environment.*/
  Environment* environment;
  Cell* cell;

  Ogre::Entity* entity;
  Ogre::SceneNode* node;

  /*! Player Movement.*/
  Ogre::Vector3 velocity;
  bool moveForward;
  bool moveBack;
  bool moveLeft;
  bool moveRight;
  bool run;
  bool leftHand;
  bool rightHand;
  Ogre::Vector3 leftHandTarget;
  Ogre::Vector3 rightHandTarget;
  Ogre::Vector3 gravityVector;
  Ogre::Real speed;

  void stop();
  void moveLeftHand(const OIS::MouseEvent& evt);
  void moveRightHand(const OIS::MouseEvent& evt);

  /*! Player Physics.*/
  OgreBulletDynamics::DynamicsWorld* physics;
  OgreBulletCollisions::CapsuleCollisionShape* capsule;
  OgreBulletDynamics::RigidBody* capsuleBody;
};

