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
    material(),
    physical(NULL),
    shape(NULL)
{
}

//-------------------------------------------------------------------------------------
void Item::hasSceneChange()
{
  if(oldScene && node) oldScene->getSceneManager()->destroySceneNode(node);
  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();

  //Visual
  if(oldScene && entity) oldScene->getSceneManager()->destroyEntity(entity);
  entity = NULL;
  entity = scene->getSceneManager()->createEntity(desc.mesh);
  node->attachObject(entity);
  node->setVisible(true);

  //Physical
  simplifiedEntity = scene->getSceneManager()->createEntity(desc.simplifiedMesh);//create the simplified mesh
  if(physical) physical->release();//also releases shape
  loadPhysical();

  setUserData((IdentificationInterface*) this);
  setGroup(ITEM);
}

//-------------------------------------------------------------------------------------
Item::~Item(void)
{
  if(physical) physical->release();

  if(scene)
  {
    if(entity) scene->getSceneManager()->destroyEntity(entity);

    if(node) scene->getSceneManager()->destroySceneNode(node);
    node = NULL;
  }
}

//-------------------------------------------------------------------------------------
bool operator==(const Item& x, const Item& y)
{
  return x.name == y.name;
}

//-------------------------------------------------------------------------------------
void Item::update(double elapsedSeconds)
{
  if(physical->isSleeping()) return;//no need to update the object if it has not moved
  physx::PxTransform transform = physical->getGlobalPose();
  node->setPosition(Ogre::Vector3(transform.p.x, transform.p.y, transform.p.z));
  node->setOrientation(Ogre::Quaternion(transform.q.w,transform.q.x,transform.q.y,transform.q.z));
}

//-------------------------------------------------------------------------------------
void Item::setPosition(Ogre::Vector3 position)
{
  if(node && physical) 
  {
    node->setPosition(position);
    physical->setGlobalPose(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), physical->getGlobalPose().q));
  }
  else throw NHException("item must be inside a scene to set its position");
}

//-------------------------------------------------------------------------------------
void Item::setRotation(Ogre::Quaternion rotation)
{
  if(node && physical) 
  {
    node->setOrientation(rotation);
    physical->setGlobalPose(physx::PxTransform(physical->getGlobalPose().p, physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w)));
  }
  else throw NHException("item must be inside a scene to set its rotation");
}

//-------------------------------------------------------------------------------------
void Item::setVelocity(Ogre::Vector3 velocity)
{
  if(physical) physical->setLinearVelocity(physx::PxVec3(velocity.x, velocity.y, velocity.z));
  else throw NHException("item must be inside a scene to set its velocity");
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Item::getPosition()
{
  if(node) return node->getPosition();
  throw NHException("item must be inside a scene to get its position");
}

//-------------------------------------------------------------------------------------
Ogre::Quaternion Item::getRotation()
{
  if(node) return node->getOrientation();
  throw NHException("item must be inside a scene to get its rotation");
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Item::getVelocity()
{
  if(physical)
  {
    physx::PxVec3 vel = physical->getLinearVelocity();
    return Ogre::Vector3(vel.x, vel.y, vel.z);
  }
  throw NHException("item must be inside a scene to get its velocity");
}

//-------------------------------------------------------------------------------------
ItemDesc Item::getDescription()
{
  return desc;
}

//-------------------------------------------------------------------------------------
void Item::setUserData(void* target)
{
  if(physical) physical->userData = target;
}

//-------------------------------------------------------------------------------------
void Item::setGroup(Group group)
{
  if(!shape) return;
  physx::PxFilterData filter;
  filter.word0 = group;
  shape->setQueryFilterData(filter);
}

//-------------------------------------------------------------------------------------
void Item::loadPhysical()//move to meshcomponent
{
  physical = scene->getPhysicsManager()->getPhysics().createRigidDynamic(physx::PxTransform(physx::PxVec3(0.0f,0.0f,0.0f)));
  if(!physical) throw NHException("could not create item physics actor");
  if(!material) material = scene->getPhysicsManager()->getPhysics().createMaterial(desc.staticFriction, desc.dynamicFriction, desc.restitution);//need to index materials so i'm not creating them for every item instance  //scene->getWorld()->getDefaultPhysicsMaterial();
  //shape = physical->createShape(physx::PxBoxGeometry(side, side, side), *material);//temporary simplified collision mesh
  shape = physical->createShape(physx::PxConvexMeshGeometry(scene->getWorld()->getFabricationManager()->createConvexMesh(entity->getMesh())), *material);//temporary complex collision mesh
  physical->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
  scene->getPhysicsManager()->addActor(*physical);
  physx::PxRigidBodyExt::updateMassAndInertia(*physical, desc.density);
}



