#pragma once
#include "NodeComponent.h"

class MovementComponent : public NodeComponent
{
public:
  MovementComponent(Ogre::Vector3 gravity);
  virtual ~MovementComponent(void);

  virtual void update(double elapsedSeconds) = 0;

  //Movement
  void stop();
  void jump();

  //Getters
  Ogre::Vector3 getVelocity();
  float getSpeed();

  //Setters
  void setRun(bool run);
  void setGravity(Ogre::Vector3 gravity);
  void setGravityEnabled(bool enabled);
  
  //Assertions
  bool isRunning();
  bool isJumping();

  void hitGround();

protected:
  virtual void hasNodeChange();

  //Movement Flags
  bool running;
  bool jumping;

  //State Data
  Ogre::Vector3 velocity;
  float speed;
  Ogre::Vector3 gravity;
  bool gravityEnabled;

  //General Constants
  float runScalar;
  float moveScalar;
  float jumpVelocity;
  float minimumMoveDistance;
};

