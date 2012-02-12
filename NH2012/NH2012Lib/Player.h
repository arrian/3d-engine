#pragma once

#include <Ogre.h>
#include <limits>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "Actor.h"
#include "Bar.h"
#include "Inventory.h"
#include "Attributes.h"
#include "Dungeon.h"
#include "Flag.h"

//character physics
#include "Shapes\OgreBulletCollisionsCapsuleShape.h"
#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"

class Dungeon;

/*! User data.*/
class Player : public Actor
{
public:
  Player(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
         Flag* flags, Ogre::RenderWindow* window, Ogre::Vector3 position);
  ~Player(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void injectKeyDown(const OIS::KeyEvent &evt);
  void injectKeyUp(const OIS::KeyEvent &evt);
  void injectMouseMove(const OIS::MouseEvent &evt);
  void injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

  /*! Gets the current position of the player.*/
  Ogre::Vector3 getPosition();

private:
  /*! Used in hardcore mode .*/
  Bar water;
  Bar food;
  Bar sleep;

  Ogre::SceneNode* cameraNode;
  Ogre::Camera* camera;
  Ogre::Real playerHeight;//!

  Flag* flags;

  void animation();
  void audio();
  void collision();

  Ogre::Vector3 gravityVector;

  //Camera movement stuff
  Ogre::Vector3 velocity;
  bool moveForward;
  bool moveBack;
  bool moveLeft;
  bool moveRight;
  bool run;

  bool leftHand;
  bool rightHand;

  void stop();//manually stops the player movement

  //physics
  OgreBulletCollisions::CapsuleCollisionShape* capsule;
  OgreBulletDynamics::RigidBody* capsuleBody;
};

