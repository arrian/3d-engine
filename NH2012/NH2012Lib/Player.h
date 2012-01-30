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

class Player : public Actor
{
public:
  Player(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, Ogre::Vector3 position, Dungeon* dungeon = 0);
  ~Player(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);
  void setDungeon(Dungeon* dungeon);

  void injectKeyDown(const OIS::KeyEvent &evt);
  void injectKeyUp(const OIS::KeyEvent &evt);
  void injectMouseMove(const OIS::MouseEvent &evt);
  void injectMouseDown(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
private:
  //hardcore mode
  Bar water;
  Bar food;
  Bar sleep;

  Dungeon* currentDungeon;

  Ogre::Camera* camera;
  //OgreBites::SdkCameraMan* cameraMovement;

  void animation();
  void audio();
  void collision();

  //Camera movement stuff
  Ogre::Real mTopSpeed;
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

