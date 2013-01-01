#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include "Vector3.h"

#include "PathfindManager.h"

#include "HumanoidSkeletonComponent.h"
#include "DynamicMovementComponent.h"
#include "IntelligenceComponent.h"
#include "MeshComponent.h"
#include "Bar.h"
#include "Group.h"

class Scene;

class Monster : public Actor
{
public:
  Monster(MonsterDesc description);
  virtual ~Monster(void);

  virtual void update(double elapsedSeconds);

  //Actions
  virtual void stop();
  virtual void stagger(Vector3 direction);
  virtual void damage(double amount);
  virtual void heal(double amount);
  
  //Setters
  virtual void setPosition(Vector3 position);
  virtual void setRotation(Quaternion rotation);
  virtual void setLookAt(Vector3 lookAt);
  virtual void setRunning(bool running);
  virtual void setCrouching(bool crouching);
  virtual void setGravity(Vector3);
  void setGoal(Goal* goal);

  //Getters
  virtual Vector3 getPosition();
  virtual Quaternion getRotation();
  virtual bool getCrouching();
  virtual bool getRunning();
  virtual Vector3 getGravity();

protected:
  MonsterDesc description;

  IntelligenceComponent intelligence;
  HumanoidSkeletonComponent skeleton;
  //DynamicMovementComponent movement;
  MeshComponent mesh;

  virtual void hasSceneChange();
};

