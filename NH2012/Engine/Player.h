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

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void injectKeyDown(const OIS::KeyEvent &evt);
  void injectKeyUp(const OIS::KeyEvent &evt);
  void injectMouseMove(const OIS::MouseEvent &evt);
  void injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

  /*! Gets the current position of the player.*/
  Ogre::Vector3 getPosition();

private:
  int oldCameraWidth;
  int oldCameraHeight;

  Ogre::SceneNode* cameraNode;
  Ogre::Camera* camera;
  Ogre::Real playerHeight;//!

  Environment* environment;
  Cell* cell;

  Ogre::Vector3 gravityVector;

  Ogre::Real speed;

  //Camera movement stuff
  Ogre::Vector3 velocity;
  bool moveForward;
  bool moveBack;
  bool moveLeft;
  bool moveRight;
  bool run;

  bool leftHand;
  bool rightHand;

  /*! Manually stops the player movement.*/
  void stop();

  Inventory inventory;

  
  Ogre::Entity* entity;
  Ogre::SceneNode* node;

  Ogre::RenderWindow* window;
  Ogre::Viewport* vp;
  OgreBulletDynamics::DynamicsWorld* physics;

  //physics
  OgreBulletCollisions::CapsuleCollisionShape* capsule;
  OgreBulletDynamics::RigidBody* capsuleBody;
};

