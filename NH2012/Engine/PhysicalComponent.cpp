#include "PhysicalComponent.h"

#include "extensions/PxExtensionsAPI.h"
#include "OgreSceneNode.h"
#include "Scene.h"
#include "World.h"

//-------------------------------------------------------------------------------------
PhysicalComponent::PhysicalComponent(Group group, float density)
  : NodeComponent(),
    group(group),
    actor(NULL),
    shapes(),
    density(density),
    addingShapes(false),
    userData(NULL)
{
}

//-------------------------------------------------------------------------------------
PhysicalComponent::~PhysicalComponent(void)
{
  for(std::vector<PhysicalShape*>::iterator iter = shapes.begin(); iter != shapes.end(); ++iter) delete *iter;
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::update(double elapsedSeconds)
{
  assert(actor);
  if(actor->isSleeping()) return;//no need to update the object if it has not moved
  physx::PxTransform transform = actor->getGlobalPose();
  node->setPosition(Vector3(transform.p.x, transform.p.y, transform.p.z));
  node->setOrientation(Quaternion(transform.q.w,transform.q.x,transform.q.y,transform.q.z));
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::hasNodeChange()
{
  createActor();
  
  for(std::vector<PhysicalShape*>::iterator iter = shapes.begin(); iter != shapes.end(); ++iter)
  {
    (*iter)->attach(actor);
  }

  scene->getPhysicsManager()->addActor(*actor);
  physx::PxRigidBodyExt::updateMassAndInertia(*actor, density);
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::setUserData(void* data)
{
  userData = data;
  if(actor) actor->userData = data;
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::createActor()
{
  if(actor) actor->release();
  Vector3 oPosition = node->getPosition();
  physx::PxVec3 pPosition = physx::PxVec3(oPosition.x, oPosition.y, oPosition.z);
  actor = scene->getPhysicsManager()->getPhysics().createRigidDynamic(physx::PxTransform(pPosition));
  if(!actor) throw NHException("could not create physical component actor");
  actor->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
  actor->userData = userData;
}

//-------------------------------------------------------------------------------------
physx::PxRigidDynamic* PhysicalComponent::getActor()
{
  return actor;
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::begin()
{
  addingShapes = true;
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::addConvexMesh(physx::PxConvexMesh* mesh, physx::PxMaterial* material, Vector3 offset)
{
  shapes.push_back(new ConvexShape(mesh, group, material, offset));
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::addBoxMesh(float length, float width, float height, physx::PxMaterial* material, Vector3 offset)
{
  shapes.push_back(new BoxShape(length, width, height, group, material, offset));
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::addSphereMesh(float radius, physx::PxMaterial* material, Vector3 offset)
{
  shapes.push_back(new SphereShape(radius, group, material, offset));
}

//-------------------------------------------------------------------------------------
void PhysicalComponent::end()
{
  addingShapes = false;
  if(node != NULL) hasNodeChange();
}





