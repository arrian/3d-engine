#pragma once
#include "NodeComponent.h"

class KinematicMovementComponent : public NodeComponent
{
public:
  KinematicMovementComponent(void);
  virtual ~KinematicMovementComponent(void);

  virtual void update(double elapsedSeconds);

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
  void setMoveForward(bool state);
  void setMoveBackward(bool state);
  void setMoveLeft(bool state);
  void setMoveRight(bool state);

  //Assertions
  bool isRunning();
  bool isJumping();

protected:
  virtual void hasNodeChange();

  //Movement Flags
  bool moveForward;
  bool moveBack;
  bool moveLeft;
  bool moveRight;
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

