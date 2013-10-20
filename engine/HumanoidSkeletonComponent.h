#pragma once

//Ogre
#include "Vector3.h"
#include <OgreSceneNode.h>

//PhysX
#include "PxPhysicsAPI.h"
#include "characterkinematic/PxCapsuleController.h"
#include "characterkinematic/PxControllerBehavior.h"
#include "characterkinematic/PxController.h"

//Local
#include "Item.h"
#include "NodeComponent.h"
#include "Group.h"

/************************************************************************/
/* A humanoid skeleton that can be attached to a node.                  */
/************************************************************************/
class HumanoidSkeletonComponent : public NodeComponent, public physx::PxControllerBehaviorCallback, public physx::PxUserControllerHitReport
{
public:
  HumanoidSkeletonComponent(void);
  ~HumanoidSkeletonComponent(void);

  void update(double elapsedSeconds);

  //Getters
  Ogre::SceneNode* getHead();
  Ogre::SceneNode* getLeftHand();
  Ogre::SceneNode* getRightHand();
  physx::PxU32 getBehaviorFlags(const physx::PxShape& shape);
  physx::PxU32 getBehaviorFlags(const physx::PxController& controller);
  physx::PxU32 getBehaviorFlags(const physx::PxObstacle& obstacle);
  Vector3 getForwardPosition(Ogre::Real distance);/** Gets a position in front of the skeleton that is the given distance from it. */

  //Setters
  void setLeftHand(bool state);
  void setRightHand(bool state);
  void setCollisionEnabled(bool enabled);
  void setCrouch(bool crouch);
  void setUserData(void* userData);
  void setGroup(Group group);
  //void setRagdoll(bool isRagdoll);

  //Movement
  void stop();
  void headRelative(Ogre::Degree x, Ogre::Degree y);
  void leftHandRelative(Ogre::Degree x, Ogre::Degree y);
  void rightHandRelative(Ogre::Degree x, Ogre::Degree y);
  void lookAt(Vector3 position);

  //Assertions
  bool isLeftHand();
  bool isRightHand();
  bool isCrouched();
  bool isOnGround();

  //Callbacks
  void onShapeHit(const physx::PxControllerShapeHit& hit);
	void onControllerHit(const physx::PxControllersHit& hit) {}
	void onObstacleHit(const physx::PxControllerObstacleHit& hit) {}

protected:
  void* userData;
  Group group;

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
  Vector3 headOrigin;
  Vector3 headCrouchOrigin;
  Vector3 leftHandOrigin;
  Vector3 rightHandOrigin;

  //Movement Flags
  bool moveLeftHand;
  bool moveRightHand;
  bool onGround;
  bool crouch;

  void hasNodeChange();

  //State Data
  bool collisionEnabled;
  Vector3 previousPosition;//previous update position
  
  //PhysX Constants
  float radius;
  float height;
  float density;
  float scaleCoeff;
  float stepOffset;
  float minimumMoveDistance;

  bool canStand();//determines if there is enough space above the skeleton to change from the crouching/lying position to standing
};

