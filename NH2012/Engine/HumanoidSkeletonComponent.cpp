#include "HumanoidSkeletonComponent.h"

#include "Scene.h"
#include "extensions/PxRigidBodyExt.h"

HumanoidSkeletonComponent::HumanoidSkeletonComponent(void)
  : NodeComponent(),
    physx::PxControllerBehaviorCallback(),
    controller(0),
    speed(40),
    gravity(-98.1f),
    velocity(Ogre::Vector3::ZERO)
{
}


HumanoidSkeletonComponent::~HumanoidSkeletonComponent(void)
{
}

void HumanoidSkeletonComponent::hasNodeChange()
{
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
  desc.radius = 30;
  desc.height = 80;
  desc.density = 10;
  desc.position = physx::PxExtendedVec3(node->getPosition().x, node->getPosition().y, node->getPosition().z);
  desc.scaleCoeff = 1;
  desc.behaviorCallback = this;
  desc.callback = this;

  //set character controller desc options here
  controller = scene->getControllerManager()->createController(scene->getPhysicsManager()->getPhysics(), scene->getPhysicsManager(), desc);

  physx::PxControllerManager* m = scene->getControllerManager();

  if(!controller) throw NHException("Could not create character kinematic.");

  stop();

  head = node->createChildSceneNode();
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
    velocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
  }
  else velocity -= velocity * evt.timeSinceLastFrame * 30;

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

  physx::PxU32 collisionFlags = controller->move(physx::PxVec3(velocity.x * evt.timeSinceLastFrame * 10.0f, velocity.y * evt.timeSinceLastFrame * 10.0f, velocity.z * evt.timeSinceLastFrame * 10.0f), 0.5f, evt.timeSinceLastFrame, physx::PxControllerFilters());//moving character controller
  
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
  velocity.y = 25.0f;
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

/*
void Player::Update(double timeSinceLastFrame)
{
   updateAnimations(timeSinceLastFrame);
   
   //|||||||||||||||||||||||||||||||||||||||||||||||
   // Turn
   //|||||||||||||||||||||||||||||||||||||||||||||||

   if (bTurnCharacter)
   {
      gYaw += gTurn * timeSinceLastFrame * gCharacterTurnSpeed;
      mController->setDisplayYaw(gYaw);
      bTurnCharacter = false;
   }

   //|||||||||||||||||||||||||||||||||||||||||||||||
   // Movement
   //|||||||||||||||||||||||||||||||||||||||||||||||
   NxOgre::Vec3 disp(0,-GRAVITY,0); // default gravity

   float rotation = mController->getDisplayYaw();
   rotation *= 3.141592654f / 180; // Convert to radians
   
   if(bPushCharacter)
   {
      NxOgre::Vec3 horizontalDisp;;
      horizontalDisp.y = 0.0f;
      horizontalDisp.x = gCharacterVec.z * NxOgre::Math::sin(rotation) + gCharacterVec.x * NxOgre::Math::cos(rotation);
      horizontalDisp.z = gCharacterVec.z * NxOgre::Math::cos(-rotation) + gCharacterVec.x * NxOgre::Math::sin(-rotation);
      horizontalDisp.normalise();
      disp += horizontalDisp * gCharacterMoveSpeed;
      bPushCharacter = false;
   }

   disp *= timeSinceLastFrame;

   //|||||||||||||||||||||||||||||||||||||||||||||||
   // Jump
   //|||||||||||||||||||||||||||||||||||||||||||||||

   float height = GetHeight(timeSinceLastFrame); // compute height(Y) in jumping
   if (height != 0.0f)
   {
      disp.y += height;
   }
   
   unsigned int collisionFlags;
   mController->move(disp,COLLIDABLE_MASK,0.000001f,collisionFlags,1.0f);
   if(collisionFlags & NxOgre::Enums::ControllerFlag_Down)
      StopJump(); //stop jump
}


void Player::move(NxOgre::Vec3 dir)
{
   gCharacterVec = dir;
   bPushCharacter = true;
}

void Player::jump()
{
   if (gJump)  return;
   gJumpTime = 0.0f;
   gV0   = gJumpSpeed;
   setBaseAnimation(ANIM_JUMP_START, true);
   setTopAnimation(ANIM_NONE);
}

void Player::turn(float f)
{
   gTurn = f;
   bTurnCharacter = true;
}

float Player::GetHeight(double elapsedTime)
{
    if (!gJump)  return 0.0f;
    float Vt = gV0 - GRAVITY*gJumpTime; // Vt = Vo + GT
    gJumpTime += elapsedTime;
    return Vt*elapsedTime - 1/2*GRAVITY*elapsedTime*elapsedTime; // S = VtT + 1/2GT^2
   return 0.0f;
}

void Player::StopJump()
{
   gJump = false;
   if (mBaseAnimID == ANIM_JUMP_LOOP)
      setBaseAnimation(ANIM_JUMP_END, true);
}
*/

