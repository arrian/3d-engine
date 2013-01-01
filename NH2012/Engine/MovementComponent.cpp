#include "MovementComponent.h"


MovementComponent::MovementComponent(Vector3 gravity)
  : NodeComponent(),
    speed(1.0f),
    gravity(gravity),
    velocity(Vector3::ZERO),
    gravityEnabled(true),
    runScalar(1.8f),
    moveScalar(10.0f),
    jumpVelocity(2.5f),
    minimumMoveDistance(0.001f)
{
  stop();
}

MovementComponent::~MovementComponent(void)
{
}

void MovementComponent::hasNodeChange()
{

}

void MovementComponent::stop()
{
  running = false;
  jumping = false;
  velocity = Vector3::ZERO;
}

void MovementComponent::jump()
{
  if(gravityEnabled) velocity.y = jumpVelocity;//unpredictable behaviour at low frame rates??
}

Vector3 MovementComponent::getVelocity()
{
  return velocity;
}

Vector3 MovementComponent::getGravity()
{
  return gravity;
}

float MovementComponent::getSpeed()
{
  return speed;
}

void MovementComponent::setRun(bool run)
{
  running = run;
}

void MovementComponent::setGravity(Vector3 gravity)
{
  this->gravity = gravity;
}

void MovementComponent::setGravityEnabled(bool enabled)
{
  gravityEnabled = enabled;
}

bool MovementComponent::isRunning()
{
  return running;
}

bool MovementComponent::isJumping()
{
  return jumping;
}

void MovementComponent::hitGround()
{
  velocity.y = 0.0f;
}

