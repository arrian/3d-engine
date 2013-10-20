#pragma once

#include "NodeComponent.h"
#include "Vector3.h"

class MovementComponent : public NodeComponent
{
public:
  MovementComponent(Vector3 gravity);
  virtual ~MovementComponent(void);

  virtual void update(double elapsedSeconds) = 0;

  //Movement
  virtual void stop();
  void jump();

  //Getters
  Vector3 getVelocity();
  Vector3 getGravity();
  float getSpeed();

  //Setters
  void setRun(bool run);
  void setGravity(Vector3 gravity);
  void setGravityEnabled(bool enabled);
  
  //Assertions
  bool isRunning();
  bool isJumping();

  void hitGround();

  void push(Vector3 strength);

protected:
  virtual void hasNodeChange();

  //Movement Flags
  bool running;
  bool jumping;

  //State Data
  Vector3 velocity;
  float speed;
  Vector3 gravity;
  bool gravityEnabled;

  //General Constants
  float runScalar;
  float moveScalar;
  float jumpVelocity;
  float minimumMoveDistance;
};

