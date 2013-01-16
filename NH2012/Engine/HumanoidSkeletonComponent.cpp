#include "HumanoidSkeletonComponent.h"

#include "World.h"
#include "Scene.h"
#include "extensions/PxRigidBodyExt.h"

//-------------------------------------------------------------------------------------
HumanoidSkeletonComponent::HumanoidSkeletonComponent(void)
  : NodeComponent(),
    physx::PxControllerBehaviorCallback(),
    controller(NULL),
    leftHandItem(NULL),
    rightHandItem(NULL),
    headItem(NULL),
    leftHandOrigin(0.0f,-0.5f,0.5f),//test value
    rightHandOrigin(0.0f,0.5f,0.5f),//test value
    headOrigin(0.0f,1.4f,0.0f),
    headCrouchOrigin(0.0f,0.7f,0.0f),
    collisionEnabled(true),
    moveLeftHand(false),
    moveRightHand(false),
    radius(0.5f),
    height(1.75f),
    density(1.0f),
    scaleCoeff(0.999f),
    stepOffset(0.4f),
    minimumMoveDistance(0.001f),
    userData(NULL),
    onGround(false),
    crouch(false),
    previousPosition(Vector3::ZERO)
{
}

//-------------------------------------------------------------------------------------
HumanoidSkeletonComponent::~HumanoidSkeletonComponent(void)
{
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::hasNodeChange()
{
  if(oldScene)
  {
    if(head) oldScene->getGraphicsManager()->destroySceneNode(head);
    if(rightHand) oldScene->getGraphicsManager()->destroySceneNode(rightHand);
    if(leftHand) oldScene->getGraphicsManager()->destroySceneNode(leftHand);

    head = NULL;
    rightHand = NULL;
    leftHand = NULL;
  }

  //pulling down
  if(controller) controller->release();
  controller = NULL;
  //also destroy actor

  //setting up

  if(!scene || !node) return;

  previousPosition = node->_getDerivedPosition();

  physx::PxCapsuleControllerDesc desc;
  desc.material = scene->getWorld()->getPhysicsManager()->getDefaultMaterial();
  desc.radius = radius;
  desc.height = height;
  desc.density = density;
  desc.scaleCoeff = scaleCoeff;
  desc.stepOffset = stepOffset;
  desc.position = physx::PxExtendedVec3(node->getPosition().x, node->getPosition().y, node->getPosition().z);
  desc.behaviorCallback = this;
  desc.callback = this;
  desc.userData = userData;
 
  //creating the controller//make sure casting to correct controller type
  controller = (physx::PxCapsuleController*) scene->getControllerManager()->createController(scene->getPhysicsManager()->getPhysics(), scene->getPhysicsManager(), desc);
  if(!controller) throw NHException("could not create physics character controller");
  controller->getActor()->userData = userData;

  stop();

  head = node->createChildSceneNode();
  head->setPosition(headOrigin);
  rightHand = node->createChildSceneNode();
  leftHand = node->createChildSceneNode();
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::stop()
{
  rightHand = false;
  leftHand = false;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::update(double elapsedSeconds)
{
  if(!node) return;

  if(previousPosition == node->_getDerivedPosition()) return;//the node has not moved so there is no need to update the character controller
  
  if(collisionEnabled)
  {
    Vector3 distance = node->_getDerivedPosition() - previousPosition;
    physx::PxU32 collisionFlags = controller->move(physx::PxVec3(distance.x, distance.y, distance.z), minimumMoveDistance, (float) elapsedSeconds, physx::PxControllerFilters());//moving character controller
    if((collisionFlags & physx::PxControllerFlag::eCOLLISION_DOWN) != 0) onGround = true;
    else onGround = false;
    physx::PxExtendedVec3 cPosition = controller->getPosition();
    node->setPosition(Ogre::Real(cPosition.x), Ogre::Real(cPosition.y), Ogre::Real(cPosition.z));//updating the body's visual position from the physics world calculated position
  }
  else //just move the controller ignoring all collisions
  {
    Vector3 position = node->_getDerivedPosition();
    controller->setPosition(physx::PxExtendedVec3(position.x, position.y, position.z));
  }

  previousPosition = node->_getDerivedPosition();
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setLeftHand(bool state)
{
  moveLeftHand = state;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setRightHand(bool state)
{
  moveRightHand = state;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::headRelative(Ogre::Degree x, Ogre::Degree y)
{
  node->yaw(x);
  head->pitch(y);
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::rightHandRelative(Ogre::Degree x, Ogre::Degree y)
{

}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::leftHandRelative(Ogre::Degree x, Ogre::Degree y)
{

}

//-------------------------------------------------------------------------------------
Ogre::SceneNode* HumanoidSkeletonComponent::getHead()
{
  return head;
}

//-------------------------------------------------------------------------------------
physx::PxU32 HumanoidSkeletonComponent::getBehaviorFlags(const physx::PxShape& shape)
{
  return 0;
}

//-------------------------------------------------------------------------------------
physx::PxU32 HumanoidSkeletonComponent::getBehaviorFlags(const physx::PxController& controller)
{
  return 0;
}

//-------------------------------------------------------------------------------------
physx::PxU32 HumanoidSkeletonComponent::getBehaviorFlags(const physx::PxObstacle& obstacle)
{
  return 0;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::onShapeHit(const physx::PxControllerShapeHit& hit)
{
  physx::PxRigidDynamic* dActor = hit.shape->getActor().isRigidDynamic();
  if(dActor) 
  {
		if(hit.dir.y == 0.0f)//purpose of this value??
		{
			physx::PxReal coeff = dActor->getMass() * hit.length;
			physx::PxRigidBodyExt::addForceAtLocalPos(*dActor, hit.dir * coeff, physx::PxVec3(0,0,0), physx::PxForceMode::eIMPULSE);
		}
  }
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setCollisionEnabled(bool enabled)
{
  collisionEnabled = enabled;
}

//-------------------------------------------------------------------------------------
Vector3 HumanoidSkeletonComponent::getForwardPosition(Ogre::Real distance)
{
  Vector3 position = node->getOrientation().zAxis();
  position *= distance;
  position = node->getPosition() - position;
  return position;
}

//-------------------------------------------------------------------------------------
bool HumanoidSkeletonComponent::isLeftHand()
{
  return moveLeftHand;
}

//-------------------------------------------------------------------------------------
bool HumanoidSkeletonComponent::isRightHand()
{
  return moveRightHand;
}

//-------------------------------------------------------------------------------------
bool HumanoidSkeletonComponent::isOnGround()
{
  return onGround;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setUserData(void* userData)
{
  this->userData = userData;
  if(controller) controller->getActor()->userData = userData;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setGroup(Group group)
{
  this->group = group;
  if(controller) controller->setGroupsBitmask(group);
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setCrouch(bool crouch)
{
  if(crouch)
  {
    controller->setHeight(height/2);//note: relies on falling for interpolation between crouching and standing
    head->setPosition(headCrouchOrigin);
    this->crouch = true;
  }
  else//stand
  {
    //scene->getPhysicsManager()->overlapAny(controller->getActor()->)
    controller->setPosition(controller->getPosition() + physx::PxExtendedVec3(0, height/4, 0));
    //node->translate(0.0f, height/2, 0.0f);//height is modified from the centre so the controller needs to be moved up
    controller->setHeight(height);//need to check that there is no geometry above using overlapAny
    head->setPosition(headOrigin);
    std::cout << "warning: not checking for overlaps when swapping from crouching to standing." << std::endl;
    this->crouch = false;
  }
}

//-------------------------------------------------------------------------------------
bool HumanoidSkeletonComponent::isCrouched()
{
  return crouch;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::lookAt(Vector3 position)
{
  //head->lookAt(position);
}




