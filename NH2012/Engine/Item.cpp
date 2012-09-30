#include "Item.h"

#include "World.h"
#include "Scene.h"
#include "extensions/PxExtensionsAPI.h"
#include "OgreSceneNode.h"
#include "geometry/PxConvexMeshGeometry.h"

//-------------------------------------------------------------------------------------
Item::Item(ItemDesc desc)
  : BasicComponent(),
    PhysicalInterface(0, desc.name, "Item"),
    mesh(desc.mesh),
    entity(NULL),
    node(NULL),
    friction(0.7f),
    restitution(0.5f),
    material(NULL),
    physical(NULL),
    shape(NULL),
    tempCollisionDensity(0.1f)
{
  setPosition(Ogre::Vector3::ZERO);
  setRotation(Ogre::Quaternion::IDENTITY);
}

//-------------------------------------------------------------------------------------
void Item::hasSceneChange()
{
  if(oldScene && node) oldScene->getSceneManager()->destroySceneNode(node);
  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  node->setOrientation(rotation);

  //Visual
  if(oldScene && entity) oldScene->getSceneManager()->destroyEntity(entity);
  entity = NULL;
  entity = scene->getSceneManager()->createEntity(mesh);
  node->attachObject(entity);
  node->setVisible(true);

  //Physical
  if(!material) material = scene->getWorld()->getDefaultPhysicsMaterial();
  if(physical) physical->release();//also releases shape
  loadPhysical();

  mapPhysical((PhysicalInterface*) this);
}

//-------------------------------------------------------------------------------------
Item::~Item(void)
{
  if(scene && node) scene->getSceneManager()->destroySceneNode(node);
  node = NULL;
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
  this->position = position;
  if(node) node->setPosition(position);
}

//-------------------------------------------------------------------------------------
void Item::setRotation(Ogre::Quaternion rotation)
{
  this->rotation = rotation;
  if(node) node->setOrientation(rotation);
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Item::getPosition()
{
  return position;
}

//-------------------------------------------------------------------------------------
Ogre::Quaternion Item::getRotation()
{
  return rotation;
}

//-------------------------------------------------------------------------------------
void Item::mapPhysical(void* target)
{
  assert(physical && shape);
  physical->userData = target;
  shape->userData = target;
}

//-------------------------------------------------------------------------------------
void Item::loadPhysical()
{
  Ogre::Vector3 oPosition = node->getPosition();
  physx::PxVec3 pPosition = physx::PxVec3(oPosition.x, oPosition.y, oPosition.z);

  physical = scene->getWorld()->getPhysics()->createRigidDynamic(physx::PxTransform(pPosition));
  //shape = physical->createShape(physx::PxBoxGeometry(side, side, side), *material);//temporary simplified collision mesh
  shape = physical->createShape(physx::PxConvexMeshGeometry(scene->getWorld()->getFabricationManager()->createConvexMesh(entity->getMesh())), *material);//temporary complex collision mesh
  physical->setLinearVelocity(physx::PxVec3(0.0f, 0.0f, 0.0f));
  scene->getPhysicsManager()->addActor(*physical);
  physx::PxRigidBodyExt::updateMassAndInertia(*physical, tempCollisionDensity);
}



