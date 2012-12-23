#include "HumanoidSkeletonComponent.h"

#include "World.h"
#include "Scene.h"
#include "extensions/PxRigidBodyExt.h"

//-------------------------------------------------------------------------------------
HumanoidSkeletonComponent::HumanoidSkeletonComponent(Ogre::Vector3 gravity)
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
    speed(1.0f),
    gravity(gravity),
    velocity(Ogre::Vector3::ZERO),
    collisionEnabled(true),
    moveLeftHand(false),
    moveRightHand(false),
    radius(0.5f),
    height(1.75f),
    density(1.0f),
    scaleCoeff(0.999f),
    stepOffset(0.4f),
    runScalar(1.8f),
    moveScalar(10.0f),
    jumpVelocity(2.5f),
    minimumMoveDistance(0.001f),
    userData(NULL),
    onGround(false),
    crouch(false)//,
    //isFollowingPath(false),
    //path()
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
    if(head) oldScene->getSceneManager()->destroySceneNode(head);
    if(rightHand) oldScene->getSceneManager()->destroySceneNode(rightHand);
    if(leftHand) oldScene->getSceneManager()->destroySceneNode(leftHand);

    head = NULL;
    rightHand = NULL;
    leftHand = NULL;
  }

  //pulling down
  //if(entity) entity->detachFromParent();
  //if(scene) scene->getSceneManager()->destroySceneNode(body);
  if(controller) controller->release();
  controller = NULL;
  //also destroy body

  //setting up

  //body = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  //body->attachObject(entity);

  if(!scene || !node) return;

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
  if(!controller) throw NHException("Could not create character kinematic.");
  controller->getActor()->userData = userData;

  //std::cout << "Actor address: " << ((void*) controller->getActor()) << std::endl;
  //std::cout << "Actor user data: " << controller->getActor()->userData << std::endl;
  //std::cout << "Controller user data: " << controller->getUserData() << std::endl;
  //std::cout << "Controller address: " << ((void*) controller) << std::endl;

  stop();

  head = node->createChildSceneNode();
  head->setPosition(headOrigin);
  rightHand = node->createChildSceneNode();
  leftHand = node->createChildSceneNode();
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::stop()
{
  moveForward = false;
  moveBack = false;
  moveLeft = false;
  moveRight = false;
  run = false;
  rightHand = false;
  leftHand = false;
  velocity = Ogre::Vector3::ZERO;
  //isFollowingPath = false;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::update(double elapsedSeconds)
{
  if(!node) return;

  //Calculating the acceleration vector
  Ogre::Vector3 accel = Ogre::Vector3::ZERO;
  if (moveForward) accel -= node->getOrientation().zAxis();//equivalent to camera->getDirection()
  if (moveBack) accel += node->getOrientation().zAxis();
  if (moveRight) accel += node->getOrientation().xAxis();//equivalent to camera->getRight()
  if (moveLeft) accel -= node->getOrientation().xAxis();
  
  float oldY;
  if(collisionEnabled)//ignore up/down directional movement from camera when collision enabled
  {
    oldY = velocity.y;
    velocity.y = 0;
  }
  
  float topSpeed = run ? speed * runScalar : speed;
  if (accel.squaredLength() != 0)
  {
    accel.normalise();
    velocity += accel * topSpeed * (float) elapsedSeconds * moveScalar;
  }
  else 
  {
    Ogre::Vector3 reduce = velocity * (float) elapsedSeconds * moveScalar;
    if(velocity.squaredLength() > reduce.squaredLength()) velocity -= reduce;//comparing with reduction length reduces jitter at low frame rates
    else velocity = Ogre::Vector3::ZERO;
  }

  Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

  if (velocity.squaredLength() > topSpeed * topSpeed)//don't go over the top speed
  {
    velocity.normalise();
    velocity *= topSpeed;
  }
  else if (velocity.squaredLength() < tooSmall * tooSmall) velocity = Ogre::Vector3::ZERO;


  //temp node propagation to controller
  controller->setPosition(physx::PxExtendedVec3(node->getPosition().x, node->getPosition().y, node->getPosition().z));



  if(collisionEnabled)
  {
    velocity.y = oldY;//restoring saved up/down speed
    velocity.y += gravity.y * (float) elapsedSeconds;//only apply gravity when collision enabled

    physx::PxU32 collisionFlags = controller->move(physx::PxVec3(velocity.x * (float) elapsedSeconds * moveScalar, velocity.y * (float) elapsedSeconds * moveScalar, velocity.z * (float) elapsedSeconds * moveScalar), minimumMoveDistance, (float) elapsedSeconds, physx::PxControllerFilters());//moving character controller
    if((collisionFlags & physx::PxControllerFlag::eCOLLISION_DOWN) != 0)
    {
      onGround = true;
      velocity.y = 0.0f;//stop falling when collision at the base of the skeleton occurs
    }
    physx::PxExtendedVec3 cPosition = controller->getPosition();
    node->setPosition(Ogre::Real(cPosition.x), Ogre::Real(cPosition.y), Ogre::Real(cPosition.z));//updating the body's visual position from the physics world calculated position
  }
  else //just move the controller ignoring all collisions
  {
    controller->setPosition(controller->getPosition() + physx::PxExtendedVec3(velocity.x * (float) elapsedSeconds * moveScalar, velocity.y * (float) elapsedSeconds * moveScalar, velocity.z * (float) elapsedSeconds * moveScalar));
    physx::PxExtendedVec3 cPosition = controller->getPosition();
    node->setPosition(Ogre::Real(cPosition.x), Ogre::Real(cPosition.y), Ogre::Real(cPosition.z));//updating the body's visual position
  }

  //////////////////////////////////////////
  /*
  if(isFollowingPath)//specific to monsters
  {
    if(path.size() < 1) return;
    Ogre::Vector3 unitDirection = path[0] - node->getPosition();
    Ogre::Real distance = unitDirection.normalise();
    Ogre::Real move = speed * (float) elapsedSeconds;
    distance -= move;
    if (distance <= 0.0f)
    {
      node->setPosition(path[0]);
      isFollowingPath = false;
    }
    else
    {
      node->translate(unitDirection * move);
    }
  }
  */
  ///////////////////////////////////////////
  
  /*if (world->freeCameraDebug) 
  {
    if(velocity != Ogre::Vector3::ZERO) camera->move(velocity * evt.timeSinceLastFrame);
    node->setPosition(camera->getPosition());
  }
  else 
  {
    if(velocity != Ogre::Vector3::ZERO && capsuleBody->getLinearVelocity() != Ogre::Vector3::ZERO) capsuleBody->setLinearVelocity(velocity);
  }*/
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
void HumanoidSkeletonComponent::setRun(bool state)
{
  run = state;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setMoveRight(bool state)
{
  moveRight = state;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setMoveLeft(bool state)
{
  moveLeft = state;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setMoveBackward(bool state)
{
  moveBack = state;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setMoveForward(bool state)
{
  moveForward = state;
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
void HumanoidSkeletonComponent::jump()
{
  if(onGround) 
  {
    velocity.y = jumpVelocity;
    onGround = false;
  }
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
float HumanoidSkeletonComponent::getSpeed()
{
  return speed;
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
void HumanoidSkeletonComponent::setGravity(Ogre::Vector3 gravity)
{
  this->gravity = gravity;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::setCollisionEnabled(bool enabled)
{
  collisionEnabled = enabled;
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 HumanoidSkeletonComponent::getForwardPosition(Ogre::Real distance)
{
  Ogre::Vector3 position = node->getOrientation().zAxis();
  position *= distance;
  position = node->getPosition() - position;
  return position;
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 HumanoidSkeletonComponent::getVelocity()
{
  return velocity;
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
bool HumanoidSkeletonComponent::isRunning()
{
  return run;
}

//-------------------------------------------------------------------------------------
bool HumanoidSkeletonComponent::isJumping()
{
  return !onGround;
}

//-------------------------------------------------------------------------------------
void HumanoidSkeletonComponent::lookAt(Ogre::Vector3 position)
{
  //head->lookAt(position);
}




