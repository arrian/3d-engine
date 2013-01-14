#include "Item.h"

#include "World.h"
#include "Scene.h"
#include "extensions/PxExtensionsAPI.h"
#include "OgreSceneNode.h"
#include "geometry/PxConvexMeshGeometry.h"

//-------------------------------------------------------------------------------------
Item::Item(ItemDesc desc)
  : desc(desc),
    BasicComponent(),
    IdentificationInterface(this, desc.name, ITEM),
    entity(NULL),
    simplifiedEntity(NULL),
    node(NULL),
    material(NULL),
    physical(ITEM)
{
}

//-------------------------------------------------------------------------------------
void Item::hasSceneChange()
{
  if(oldScene)
  {
    if(node) oldScene->getSceneManager()->destroySceneNode(node);
    if(entity) oldScene->getSceneManager()->destroyEntity(entity);
  }

  node = NULL;
  entity = NULL;

  if(scene)
  {
    node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();

    entity = scene->getSceneManager()->createEntity(desc.mesh);
    node->attachObject(entity);
    node->setVisible(true);

    //Physical
    simplifiedEntity = scene->getSceneManager()->createEntity(desc.simplifiedMesh);//create the simplified mesh
    if(!material) material = scene->getPhysicsManager()->getPhysics().createMaterial(desc.staticFriction, desc.dynamicFriction, desc.restitution);
    physical.begin();
    physical.addConvexMesh(scene->getWorld()->getFabricationManager()->createConvexMesh(simplifiedEntity->getMesh()), material);
    physical.end();

    setUserData((IdentificationInterface*) this);

    physical.setNode(scene, node);
  }
}

//-------------------------------------------------------------------------------------
Item::~Item(void)
{
  //if(physical) physical->release();

  if(scene)
  {
    if(entity) scene->getSceneManager()->destroyEntity(entity);
    if(node) scene->getSceneManager()->destroySceneNode(node);
  }

  node = NULL;
  entity = NULL;
}

//-------------------------------------------------------------------------------------
void Item::update(double elapsedSeconds)
{
  physical.update(elapsedSeconds);
}

//-------------------------------------------------------------------------------------
void Item::setPosition(Vector3 position)
{
  if(node) 
  {
    node->setPosition(position);
    physical.getActor()->setGlobalPose(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), physical.getActor()->getGlobalPose().q));
  }
  else throw NHException("item must be inside a scene to set its position");
}

//-------------------------------------------------------------------------------------
void Item::setRotation(Quaternion rotation)
{
  if(node) 
  {
    node->setOrientation(rotation);
    physical.getActor()->setGlobalPose(physx::PxTransform(physical.getActor()->getGlobalPose().p, physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)));
  }
  else throw NHException("item must be inside a scene to set its rotation");
}

//-------------------------------------------------------------------------------------
void Item::setVelocity(Vector3 velocity)
{
  physical.getActor()->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
}

//-------------------------------------------------------------------------------------
Vector3 Item::getPosition()
{
  if(node) return node->getPosition();
  throw NHException("item must be inside a scene to get its position");
}

//-------------------------------------------------------------------------------------
Quaternion Item::getRotation()
{
  if(node) return node->getOrientation();
  throw NHException("item must be inside a scene to get its rotation");
}

//-------------------------------------------------------------------------------------
Vector3 Item::getVelocity()
{
  physx::PxVec3 vel = physical.getActor()->getLinearVelocity();
  return Vector3(vel.x, vel.y, vel.z);
}

//-------------------------------------------------------------------------------------
ItemDesc Item::getDescription()
{
  return desc;
}

//-------------------------------------------------------------------------------------
void Item::setUserData(void* target)
{
  physical.setUserData(target);
}

/*
//-------------------------------------------------------------------------------------
void Item::loadPhysical()//move to meshcomponent
{
  physical = scene->getPhysicsManager()->getPhysics().createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f,0.0f,0.0f)));
  if(!physical) throw NHException("could not create item physics actor");
  //need to index materials so i'm not creating them for every item instance  //scene->getWorld()->getDefaultPhysicsMaterial();
  //shape = physical->createShape(physx::PxBoxGeometry(side, side, side), *material);//temporary simplified collision mesh
  shape = physical->createShape(physx::PxConvexMeshGeometry(scene->getWorld()->getFabricationManager()->createConvexMesh(entity->getMesh())), *material);//temporary complex collision mesh
  physical->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
  scene->getPhysicsManager()->addActor(*physical);
  physx::PxRigidBodyExt::updateMassAndInertia(*physical, desc.density);
}
*/


