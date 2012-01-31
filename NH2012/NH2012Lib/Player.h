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

class Dungeon;

class Player : public Actor
{
public:
  Player(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, Ogre::RenderWindow* window, Ogre::Vector3 position);
  ~Player(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void injectKeyDown(const OIS::KeyEvent &evt);
  void injectKeyUp(const OIS::KeyEvent &evt);
  void injectMouseMove(const OIS::MouseEvent &evt);
  void injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

  Ogre::Vector3 getPosition();
private:
  //hardcore mode
  Bar water;
  Bar food;
  Bar sleep;

  Ogre::Camera* camera;
  //OgreBites::SdkCameraMan* cameraMovement;

  void animation();
  void audio();
  void collision();

  //Camera movement stuff
	Ogre::Vector3 mVelocity;
	bool mGoingForward;
	bool mGoingBack;
	bool mGoingLeft;
	bool mGoingRight;
	bool mGoingUp;
	bool mGoingDown;
	bool mFastMove;

  void stop();//manually stops the player movement
};

