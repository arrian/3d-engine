#pragma once
#include "NodeComponent.h"

#include "PxPhysicsAPI.h"


class PhysicalComponent : public NodeComponent
{
public:
  PhysicalComponent(Ogre::Real friction = 0.7f, Ogre::Real restitution = 0.5f, physx::PxMaterial* material = 0);
  ~PhysicalComponent(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  /* Provides a 1:1 mapping between the physical representation and the given object.*/
  void mapPhysical(void* target);

protected:
  void loadPhysical();//loads a simplified collision mesh

  void hasNodeChange();

  Ogre::Real friction;
  Ogre::Real restitution;
  physx::PxMaterial* material;

  physx::PxRigidDynamic* physical;
  physx::PxShape* shape;

  float tempCollisionCubeSides;
  float tempCollisionDensity;
};

