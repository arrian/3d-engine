#pragma once

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include "PxPhysicsAPI.h"
#include "characterkinematic/PxCapsuleController.h"
#include "NodeComponent.h"
#include "characterkinematic/PxControllerBehavior.h"
#include "characterkinematic/PxController.h"

class HumanoidSkeletonComponent : public NodeComponent, public physx::PxControllerBehaviorCallback, public physx::PxUserControllerHitReport
{
public:
  HumanoidSkeletonComponent(void);
  ~HumanoidSkeletonComponent(void);

  Ogre::SceneNode* getHead();
  Ogre::SceneNode* getLeftHand();
  Ogre::SceneNode* getRightHand();

  void setMoveForward(bool state);
  void setMoveBackward(bool state);
  void setMoveLeft(bool state);
  void setMoveRight(bool state);
  void setRun(bool state);

  void setLeftHand(bool state);
  void setRightHand(bool state);

  void stop();

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void headRelative(Ogre::Degree x, Ogre::Degree y);
  void leftHandRelative(Ogre::Degree x, Ogre::Degree y);
  void rightHandRelative(Ogre::Degree x, Ogre::Degree y);

  void jump();

  physx::PxU32 getBehaviorFlags(const physx::PxShape& shape);
  physx::PxU32 getBehaviorFlags(const physx::PxController& controller);
  physx::PxU32 getBehaviorFlags(const physx::PxObstacle& obstacle);

  void onShapeHit(const physx::PxControllerShapeHit& hit);
	void onControllerHit(const physx::PxControllersHit& hit) {}
	void onObstacleHit(const physx::PxControllerObstacleHit& hit) {}

  void setGravity(float gravity);

  /** Gets a position in front of the skeleton that is the given distance from it. */
  Ogre::Vector3 getForwardPosition(Ogre::Real distance);

protected:
  physx::PxController* controller;

  Ogre::SceneNode* head;
  Ogre::SceneNode* leftHand;
  Ogre::SceneNode* rightHand;

  Ogre::Vector3 headOrigin;
  Ogre::Vector3 leftHandOrigin;
  Ogre::Vector3 rightHandOrigin;

  bool moveLeftHand;
  bool moveRightHand;

  Ogre::Vector3 velocity;
  Ogre::Real speed;
  bool moveForward;
  bool moveBack;
  bool moveLeft;
  bool moveRight;
  bool run;
  
  void hasNodeChange();

  float gravity;
};

