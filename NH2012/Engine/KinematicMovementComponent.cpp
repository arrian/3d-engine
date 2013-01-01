#include "KinematicMovementComponent.h"


KinematicMovementComponent::KinematicMovementComponent(Vector3 gravity)
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
  Vector3 accel = Vector3::ZERO;
  Vector3 zAxisOrientation = node->getOrientation().zAxis();
  Vector3 xAxisOrientation = node->getOrientation().xAxis();
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
    Vector3 reduce = velocity * (float) elapsedSeconds * moveScalar;
    if(velocity.squaredLength() > reduce.squaredLength()) velocity -= reduce;//comparing with reduction length reduces jitter at low frame rates
    else velocity = Vector3::ZERO;
  }

  Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

  if (velocity.squaredLength() > topSpeed * topSpeed)//don't go over the top speed
  {
    velocity.normalise();
    velocity *= topSpeed;
  }
  else if (velocity.squaredLength() < tooSmall * tooSmall) velocity = Vector3::ZERO;

  if(gravityEnabled)//restoring saved up/down speed and adding gravity
  {
    velocity.y = oldY;
    velocity.y += gravity.y * (float) elapsedSeconds;
  }

  node->_setDerivedPosition(node->_getDerivedPosition() + velocity * (float) elapsedSeconds * moveScalar);
}

void KinematicMovementComponent::stop()
{
  MovementComponent::stop();
  moveForward = false;
  moveBack = false;
  moveLeft = false;
  moveRight = false;
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


