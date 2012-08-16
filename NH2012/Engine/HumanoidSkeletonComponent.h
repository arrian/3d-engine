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

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  //Getters
  Ogre::SceneNode* getHead();
  Ogre::SceneNode* getLeftHand();
  Ogre::SceneNode* getRightHand();
  physx::PxU32 getBehaviorFlags(const physx::PxShape& shape);
  physx::PxU32 getBehaviorFlags(const physx::PxController& controller);
  physx::PxU32 getBehaviorFlags(const physx::PxObstacle& obstacle);
  Ogre::Vector3 getForwardPosition(Ogre::Real distance);/** Gets a position in front of the skeleton that is the given distance from it. */

  //Setters
  void setMoveForward(bool state);
  void setMoveBackward(bool state);
  void setMoveLeft(bool state);
  void setMoveRight(bool state);
  void setRun(bool state);
  void setLeftHand(bool state);
  void setRightHand(bool state);
  void setGravity(float gravity);
  void setCollisionEnabled(bool enabled);

  //Movement
  void stop();
  void jump();
  void headRelative(Ogre::Degree x, Ogre::Degree y);
  void leftHandRelative(Ogre::Degree x, Ogre::Degree y);
  void rightHandRelative(Ogre::Degree x, Ogre::Degree y);

  //Callbacks
  void onShapeHit(const physx::PxControllerShapeHit& hit);
	void onControllerHit(const physx::PxControllersHit& hit) {}
	void onObstacleHit(const physx::PxControllerObstacleHit& hit) {}

protected:
  //PhysX Physical Body
  physx::PxController* controller;

  //Limb Target Nodes
  Ogre::SceneNode* head;
  Ogre::SceneNode* leftHand;
  Ogre::SceneNode* rightHand;

  //Inventory
  Item* leftHandItem;
  Item* rightHandItem;
  Item* headItem;

  //Relative Origins
  Ogre::Vector3 headOrigin;
  Ogre::Vector3 leftHandOrigin;
  Ogre::Vector3 rightHandOrigin;

  //Movement Flags
  bool moveLeftHand;
  bool moveRightHand;
  bool moveForward;
  bool moveBack;
  bool moveLeft;
  bool moveRight;
  bool run;

  void hasNodeChange();

  //State Data
  Ogre::Vector3 velocity;
  Ogre::Real speed;
  float gravity;
  bool collisionEnabled;
  
  //PhysX Constants
  float radius;
  float height;
  float density;
  float scaleCoeff;
  float stepOffset;

  float runScalar;
  float moveScalar;
  float jumpVelocity;
  float minimumMoveDistance;
};

