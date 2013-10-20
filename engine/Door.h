#pragma once

#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxRevoluteJoint.h"

#include <OgreEntity.h>

#include "Interactive.h"
#include "MeshComponent.h"
#include "PhysicalComponent.h"

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

  virtual void setPosition(Vector3 position);
  virtual void setRotation(Quaternion rotation);

  virtual Vector3 getPosition();
  virtual Quaternion getRotation();

protected:
  virtual void hasSceneChange();

private:
  bool open;
  bool locked;

  Ogre::SceneNode* node;
  Vector3 position;
  Quaternion rotation;
  
  //Door
  PhysicalComponent doorPhysical;
  MeshComponent doorMesh;
  physx::PxTransform doorConstraint;

  //Frame
  physx::PxTransform frameConstraint;

  //Hinge
  physx::PxRevoluteJoint* hingeJoint;
};

