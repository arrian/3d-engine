#pragma once

//Ogre
#include <OgreVector3.h>
#include <OgreSceneNode.h>

//PhysX
#include "PxPhysicsAPI.h"
#include "characterkinematic/PxCapsuleController.h"
#include "characterkinematic/PxControllerBehavior.h"
#include "characterkinematic/PxController.h"

//Local
#include "Item.h"
#include "NodeComponent.h"

/************************************************************************/
/* A humanoid skeleton that can be attached to a node.                  */
/************************************************************************/
class HumanoidSkeletonComponent : public NodeComponent, public physx::PxControllerBehaviorCallback, public physx::PxUserControllerHitReport
{
public:
  HumanoidSkeletonComponent(Ogre::Vector3 gravity);
  ~HumanoidSkeletonComponent(void);

  void update(double elapsedSeconds);

  void mapPhysical(void* userData);

  //Getters
  Ogre::SceneNode* getHead();
  Ogre::SceneNode* getLeftHand();
  Ogre::SceneNode* getRightHand();
  physx::PxU32 getBehaviorFlags(const physx::PxShape& shape);
  physx::PxU32 getBehaviorFlags(const physx::PxController& controller);
  physx::PxU32 getBehaviorFlags(const physx::PxObstacle& obstacle);
  Ogre::Vector3 getForwardPosition(Ogre::Real distance);/** Gets a position in front of the skeleton that is the given distance from it. */
  Ogre::Vector3 getVelocity();
  float getSpeed();

  //Setters
  void setMoveForward(bool state);
  void setMoveBackward(bool state);
  void setMoveLeft(bool state);
  void setMoveRight(bool state);
  void setRun(bool state);
  void setLeftHand(bool state);
  void setRightHand(bool state);
  void setGravity(Ogre::Vector3 gravity);
  void setCollisionEnabled(bool enabled);
  void setCrouch(bool crouch);

  //Movement
  void stop();
  void jump();
  void headRelative(Ogre::Degree x, Ogre::Degree y);
  void leftHandRelative(Ogre::Degree x, Ogre::Degree y);
  void rightHandRelative(Ogre::Degree x, Ogre::Degree y);
  void lookAt(Ogre::Vector3 position);
  //void followPath(std::vector<Ogre::Vector3> path);

  //Assertions
  bool isLeftHand();
  bool isRightHand();
  bool isCrouched();
  bool isRunning();
  bool isJumping();

  //Callbacks
  void onShapeHit(const physx::PxControllerShapeHit& hit);
	void onControllerHit(const physx::PxControllersHit& hit) {}
	void onObstacleHit(const physx::PxControllerObstacleHit& hit) {}

protected:
  void* userData;

  //PhysX Physical Body
  physx::PxCapsuleController* controller;

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
  Ogre::Vector3 headCrouchOrigin;
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
  bool onGround;
  bool crouch;

  void hasNodeChange();

  //State Data
  Ogre::Vector3 velocity;
  float speed;
  Ogre::Vector3 gravity;//note that only the y value is currently used
  bool collisionEnabled;
  
  //PhysX Constants
  float radius;
  float height;
  float density;
  float scaleCoeff;
  float stepOffset;
  float minimumMoveDistance;

  //General Constants
  float runScalar;
  float moveScalar;
  float jumpVelocity;

  //bool isFollowingPath;
  //std::vector<Ogre::Vector3> path;
};

