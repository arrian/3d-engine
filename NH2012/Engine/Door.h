#pragma once

#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxRevoluteJoint.h"

#include <OgreEntity.h>

#include "Interactive.h"

class Door : public Interactive
{
public:
  Door(void);
  virtual ~Door(void);

  void setOpen(bool open);
  void setLocked(bool locked);

  bool isOpen();
  bool isLocked();

  virtual void interact();
  virtual void update(double elapsedSeconds);

  virtual void setPosition(Ogre::Vector3 position);
  virtual void setRotation(Ogre::Quaternion rotation);

  virtual Ogre::Vector3 getPosition();
  virtual Ogre::Quaternion getRotation();

protected:
  virtual void hasSceneChange();

private:
  bool open;
  bool locked;

  Ogre::SceneNode* node;
  Ogre::Vector3 position;
  Ogre::Quaternion rotation;

  //Door
  Ogre::Entity* doorEntity;
  physx::PxShape* doorShape;
  physx::PxRigidDynamic* doorActor;
  physx::PxTransform doorConstraint;
  
  //Frame
  physx::PxTransform frameConstraint;
  
  //Hinge
  physx::PxRevoluteJoint* hingeJoint;
};

