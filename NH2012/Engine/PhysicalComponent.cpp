#include "PhysicalComponent.h"

#include "extensions/PxExtensionsAPI.h"
#include "OgreSceneNode.h"
#include "Scene.h"
#include "World.h"

//-------------------------------------------------------------------------------------
PhysicalComponent::PhysicalComponent(Ogre::Real friction, Ogre::Real restitution, physx::PxMaterial* material)
  : NodeComponent(),
    friction(friction),
    restitution(restitution),
    material(material),
    physical(NULL),
    shape(NULL),
    tempCollisionCubeSides(0.5f),
    tempCollisionDensity(0.0005f)
{
 
}

//-------------------------------------------------------------------------------------
PhysicalComponent::~PhysicalComponent(void)
{
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::update(double elapsedSeconds)
{
  //if(!physical) throw NHException("Physical component missed frame rendering because it is not yet created.");
  if(physical->isSleeping()) return;//no need to update the object if it has not moved
  physx::PxTransform transform = physical->getGlobalPose();
  node->setPosition(Ogre::Vector3(transform.p.x, transform.p.y, transform.p.z));
  node->setOrientation(Ogre::Quaternion(transform.q.w,transform.q.x,transform.q.y,transform.q.z));
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::hasNodeChange()
{
  if(!material) material = scene->getWorld()->getPhysicsManager()->getDefaultMaterial();
  if(physical) physical->release();//also releases shape

  loadPhysical();
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::mapPhysical(void* target)
{
  assert(physical && shape);
  physical->userData = target;
  shape->userData = target;
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::loadPhysical()
{
  float density = tempCollisionDensity;
  float side = tempCollisionCubeSides;

  Ogre::Vector3 oPosition = node->getPosition();
  physx::PxVec3 pPosition = physx::PxVec3(oPosition.x, oPosition.y, oPosition.z);

  physical = scene->getWorld()->getPhysicsManager()->getPhysics()->createRigidDynamic(physx::PxTransform(pPosition));
  shape = physical->createShape(physx::PxBoxGeometry(side, side, side), *material);//temporary simplified collision mesh
  physical->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
  scene->getPhysicsManager()->addActor(*physical);

  physx::PxRigidBodyExt::updateMassAndInertia(*physical, density);
}




