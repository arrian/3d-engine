#pragma once

#include "Vector3.h"
#include "Quaternion.h"
#include "BasicComponent.h"
#include "Identifiable.h"

class Scene;

/**
 * Defines an intelligent agent in the world.
 **/
class Actor : public BasicComponent, public Identifiable
{
public:
  Actor(void* instancePointer, std::string name, Group group);
  virtual ~Actor(void);

  virtual void update(double elapsedSeconds) = 0;

  //Actions
  virtual void stop() = 0;
  virtual void stagger(Vector3 direction) = 0;
  virtual void damage(double amount) = 0;
  virtual void heal(double amount) = 0;

  //Setters
  virtual void setPosition(Vector3 position) = 0;
  virtual void setRotation(Quaternion rotation) = 0;
  virtual void setLookAt(Vector3 lookAt) = 0;
  virtual void setRunning(bool running) = 0;
  virtual void setCrouching(bool crouching) = 0;
  virtual void setGravity(Vector3) = 0;

  //Getters
  virtual Vector3 getPosition() = 0;
  virtual Quaternion getRotation() = 0;
  virtual bool getCrouching() = 0;
  virtual bool getRunning() = 0;
  virtual Vector3 getGravity() = 0;

protected:
  Ogre::SceneNode* node;
  Vector3 position;

  virtual void hasSceneChange() = 0;
};

