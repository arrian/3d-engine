#include "HumanoidSkeletonComponent.h"

#include "Scene.h"
#include "extensions/PxRigidBodyExt.h"

HumanoidSkeletonComponent::HumanoidSkeletonComponent(void)
  : NodeComponent(),
    physx::PxControllerBehaviorCallback(),
    controller(0),
    speed(1),
    gravity(-9.81f),
    velocity(Ogre::Vector3::ZERO)
{
}


HumanoidSkeletonComponent::~HumanoidSkeletonComponent(void)
{
}

void HumanoidSkeletonComponent::hasNodeChange()
{
  if(oldScene)
  {
    if(head) oldScene->getSceneManager()->destroySceneNode(head);
    if(rightHand) oldScene->getSceneManager()->destroySceneNode(rightHand);
    if(leftHand) oldScene->getSceneManager()->destroySceneNode(leftHand);

    head = 0;
    rightHand = 0;
    leftHand = 0;
  }

  //pulling down
  //if(entity) entity->detachFromParent();
  //if(scene) scene->getSceneManager()->destroySceneNode(body);
  if(controller) controller->release();
  //also destroy body

  //setting up

  //body = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  //body->attachObject(entity);

  physx::PxCapsuleControllerDesc desc;
  desc.material = scene->getWorld()->getDefaultPhysicsMaterial();
  desc.radius = 0.5f;
  desc.height = 1.0f;
  desc.density = 1.0f;
  desc.position = physx::PxExtendedVec3(node->getPosition().x, node->getPosition().y, node->getPosition().z);
  desc.scaleCoeff = 1.0f;
  desc.behaviorCallback = this;
  desc.callback = this;
  desc.stepOffset = 0.4f;

  //set character controller desc options here
  controller = scene->getControllerManager()->createController(scene->getPhysicsManager()->getPhysics(), scene->getPhysicsManager(), desc);

  physx::PxControllerManager* m = scene->getControllerManager();

  if(!controller) throw NHException("Could not create character kinematic.");

  stop();

  head = node->createChildSceneNode();
  head->setPosition(Ogre::Vector3(0,1.0f,0));
  rightHand = node->createChildSceneNode();
  leftHand = node->createChildSceneNode();
}

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
}

void HumanoidSkeletonComponent::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!node) return;

  Ogre::Vector3 accel = Ogre::Vector3::ZERO;
  if (moveForward) accel -= node->getOrientation().zAxis();//equivalent to camera->getDirection()
  if (moveBack) accel += node->getOrientation().zAxis();
  if (moveRight) accel += node->getOrientation().xAxis();//equivalent to camera->getRight()
  if (moveLeft) accel -= node->getOrientation().xAxis();
  
  float oldY = velocity.y;
  velocity.y = 0;

  Ogre::Real topSpeed = run ? speed * 1.3f : speed;
  if (accel.squaredLength() != 0)
  {
    accel.normalise();
    velocity += accel * topSpeed * evt.timeSinceLastFrame * 10.0f;
  }
  else 
  {
    Ogre::Vector3 reduce = velocity * evt.timeSinceLastFrame * 10.0f;
    if(velocity.squaredLength() > reduce.squaredLength()) velocity -= reduce;//comparing with reduction length reduces jitter at low frame rates
    else velocity = Ogre::Vector3::ZERO;
  }

  Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

  if (velocity.squaredLength() > topSpeed * topSpeed)
  {
    velocity.normalise();
    velocity *= topSpeed;
  }
  else if (velocity.squaredLength() < tooSmall * tooSmall) velocity = Ogre::Vector3::ZERO;

  //std::cout << velocity << std::endl;

  velocity.y = oldY;
  velocity.y += gravity * evt.timeSinceLastFrame;

  physx::PxU32 collisionFlags = controller->move(physx::PxVec3(velocity.x * evt.timeSinceLastFrame * 10.0f, velocity.y * evt.timeSinceLastFrame * 10.0f, velocity.z * evt.timeSinceLastFrame * 10.0f), 0.001f, evt.timeSinceLastFrame, physx::PxControllerFilters());//moving character controller
  
  if((collisionFlags & physx::PxControllerFlag::eCOLLISION_DOWN) != 0) velocity.y = 0.0f;
  
  physx::PxExtendedVec3 cPosition = controller->getPosition();
  node->setPosition(cPosition.x, cPosition.y, cPosition.z);//updating the body's visual position
  
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

void HumanoidSkeletonComponent::setLeftHand(bool state)
{
  moveLeftHand = state;
}

void HumanoidSkeletonComponent::setRightHand(bool state)
{
  moveRightHand = state;
}

void HumanoidSkeletonComponent::setRun(bool state)
{
  run = state;
}

void HumanoidSkeletonComponent::setMoveRight(bool state)
{
  moveRight = state;
}

void HumanoidSkeletonComponent::setMoveLeft(bool state)
{
  moveLeft = state;
}

void HumanoidSkeletonComponent::setMoveBackward(bool state)
{
  moveBack = state;
}

void HumanoidSkeletonComponent::setMoveForward(bool state)
{
  moveForward = state;
}

void HumanoidSkeletonComponent::headRelative(Ogre::Degree x, Ogre::Degree y)
{
  node->yaw(x);
  head->pitch(y);
}

Ogre::SceneNode* HumanoidSkeletonComponent::getHead()
{
  return head;
}

void HumanoidSkeletonComponent::jump()
{
  velocity.y = 2.5f;
}

physx::PxU32 HumanoidSkeletonComponent::getBehaviorFlags(const physx::PxShape& shape)
{
  return 0;
}

physx::PxU32 HumanoidSkeletonComponent::getBehaviorFlags(const physx::PxController& controller)
{
  return 0;
}

physx::PxU32 HumanoidSkeletonComponent::getBehaviorFlags(const physx::PxObstacle& obstacle)
{
  return 0;
}

void HumanoidSkeletonComponent::onShapeHit(const physx::PxControllerShapeHit& hit)
{
  physx::PxRigidDynamic* dActor = hit.shape->getActor().isRigidDynamic();
  if(dActor) 
  {
		if(hit.dir.y == 0.0f)
		{
			physx::PxReal coeff = dActor->getMass() * hit.length;
			physx::PxRigidBodyExt::addForceAtLocalPos(*dActor,hit.dir*coeff, physx::PxVec3(0,0,0), physx::PxForceMode::eIMPULSE);
		}
  }
}

void HumanoidSkeletonComponent::setGravity(float gravity)
{
  this->gravity = gravity;
}

Ogre::Vector3 HumanoidSkeletonComponent::getForwardPosition(Ogre::Real distance)
{
  Ogre::Vector3 position = node->getOrientation().zAxis();
  position *= distance;
  position = node->getPosition() - position;
  return position;
}

