#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include "Vector3.h"

#include "PathfindManager.h"

#include "HumanoidSkeletonComponent.h"
#include "IntelligentMovementComponent.h"
#include "MeshComponent.h"
#include "Bar.h"
#include "Group.h"
#include "Packetable.h"

class Scene;

class Monster : public Actor, public Packetable<MonsterPacket>
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
  virtual void setGravity(Vector3 gravity);
  void setGoal(Goal* goal);

  //Getters
  virtual Vector3 getPosition();
  virtual Quaternion getRotation();
  virtual bool getCrouching();
  virtual bool getRunning();
  virtual Vector3 getGravity();

  //Network
  virtual void integratePacket(MonsterPacket packet);
  virtual MonsterPacket extractPacket();

protected:
  MonsterDesc description;

  IntelligentMovementComponent intelligence;
  HumanoidSkeletonComponent skeleton;
  MeshComponent mesh;

  virtual void hasSceneChange();
};

