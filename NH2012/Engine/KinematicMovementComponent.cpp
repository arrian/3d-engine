#include "KinematicMovementComponent.h"


KinematicMovementComponent::KinematicMovementComponent(Ogre::Vector3 gravity)
  : MovementComponent(gravity)
{
  stop();
}

KinematicMovementComponent::~KinematicMovementComponent(void)
{
}

void KinematicMovementComponent::hasNodeChange()
{

}

void KinematicMovementComponent::update(double elapsedSeconds)
{
  if(!node) return;

  //Calculating the acceleration vector
  Ogre::Vector3 accel = Ogre::Vector3::ZERO;
  Ogre::Vector3 zAxisOrientation = node->getOrientation().zAxis();
  Ogre::Vector3 xAxisOrientation = node->getOrientation().xAxis();
  if (moveForward) accel -= zAxisOrientation;
  if (moveBack) accel += zAxisOrientation;
  if (moveRight) accel += xAxisOrientation;
  if (moveLeft) accel -= xAxisOrientation;

  float oldY;
  if(gravityEnabled)//ignore up/down directional movement from camera when gravity enabled
  {
    oldY = velocity.y;
    velocity.y = 0;
  }

  float topSpeed = running ? speed * runScalar : speed;
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

  if(gravityEnabled)//restoring saved up/down speed and adding gravity
  {
    velocity.y = oldY;
    velocity.y += gravity.y * (float) elapsedSeconds;
  }

  node->_setDerivedPosition(node->_getDerivedPosition() + velocity * elapsedSeconds * moveScalar);
}

void KinematicMovementComponent::stop()
{
  moveForward = false;
  moveBack = false;
  moveLeft = false;
  moveRight = false;
  running = false;
  jumping = false;
  velocity = Ogre::Vector3::ZERO;
}

void KinematicMovementComponent::jump()
{
  if(gravityEnabled) velocity.y = jumpVelocity;//unpredictable behaviour at low frame rates??
}

Ogre::Vector3 KinematicMovementComponent::getVelocity()
{
  return velocity;
}

float KinematicMovementComponent::getSpeed()
{
  return speed;
}

void KinematicMovementComponent::setRun(bool run)
{
  running = run;
}

void KinematicMovementComponent::setGravity(Ogre::Vector3 gravity)
{
  this->gravity = gravity;
}

void KinematicMovementComponent::setGravityEnabled(bool enabled)
{
  gravityEnabled = enabled;
}

void KinematicMovementComponent::setMoveForward(bool state)
{
  moveForward = state;
}

void KinematicMovementComponent::setMoveBackward(bool state)
{
  moveBack = state;
}

void KinematicMovementComponent::setMoveLeft(bool state)
{
  moveLeft = state;
}

void KinematicMovementComponent::setMoveRight(bool state)
{
  moveRight = state;
}

bool KinematicMovementComponent::isRunning()
{
  return running;
}

bool KinematicMovementComponent::isJumping()
{
  return jumping;
}

void KinematicMovementComponent::hitGround()
{
  velocity.y = 0.0f;
}

