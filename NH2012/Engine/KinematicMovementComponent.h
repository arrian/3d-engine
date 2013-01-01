#pragma once
#include "MovementComponent.h"

class KinematicMovementComponent : public MovementComponent
{
public:
  KinematicMovementComponent(Vector3 gravity);
  virtual ~KinematicMovementComponent(void);

  virtual void update(double elapsedSeconds);

  //Movement
  virtual void stop();

  //Setters
  void setMoveForward(bool state);
  void setMoveBackward(bool state);
  void setMoveLeft(bool state);
  void setMoveRight(bool state);

protected:
  virtual void hasNodeChange();

  //Movement Flags
  bool moveForward;
  bool moveBack;
  bool moveLeft;
  bool moveRight;
};

