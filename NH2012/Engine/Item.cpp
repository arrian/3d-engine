#include "Item.h"

#include "World.h"
#include "Scene.h"
#include "extensions/PxExtensionsAPI.h"
#include "OgreSceneNode.h"
#include "geometry/PxConvexMeshGeometry.h"

#include "SceneGraphicsManager.h"
#include "ScenePhysicsManager.h"

//-------------------------------------------------------------------------------------
Item::Item(ItemDesc desc)
  : desc(desc),
    BasicComponent(),
    Identifiable(this, desc.name, ITEM),
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
  boost::shared_ptr<Scene> oldScene_ptr = getOldScene();
  if(oldScene_ptr)
  {
    if(node) oldScene_ptr->getSceneGraphicsManager()->destroySceneNode(node);
    if(entity) oldScene_ptr->getSceneGraphicsManager()->destroyEntity(entity);
    if(simplifiedEntity) oldScene_ptr->getWorld()->getPhysicsManager()->getFabrication()->releaseConvexMesh(simplifiedEntity->getMesh()->getName());
  }

  node = NULL;
  entity = NULL;

  boost::shared_ptr<Scene> scene_ptr = getScene();

  if(scene_ptr)
  {
    node = scene_ptr->getSceneGraphicsManager()->createSceneNode();

    entity = scene_ptr->getSceneGraphicsManager()->createEntity(desc.mesh);
    node->attachObject(entity);
    node->setVisible(true);

    //Physical
    simplifiedEntity = scene_ptr->getSceneGraphicsManager()->createEntity(desc.simplifiedMesh);//create the simplified mesh
    if(!material) material = scene_ptr->getScenePhysicsManager()->getScenePhysics()->getPhysics().createMaterial(desc.staticFriction, desc.dynamicFriction, desc.restitution);
    physical.begin();
    physical.addConvexMesh(scene_ptr->getWorld()->getPhysicsManager()->getFabrication()->createConvexMesh(simplifiedEntity->getMesh()->getName(), simplifiedEntity->getMesh()), material);
    physical.end();

    setUserData((Identifiable*) this);

    physical.setNode(scene_ptr, node);
  }
}

//-------------------------------------------------------------------------------------
Item::~Item(void)
{
  //if(physical) physical->release();

  //material->release();//TODO: might need this

  boost::shared_ptr<Scene> scene_ptr = getScene();

  if(scene_ptr)
  {
    if(entity&& scene_ptr->getSceneGraphicsManager()) scene_ptr->getSceneGraphicsManager()->destroyEntity(entity);
    if(node && scene_ptr->getSceneGraphicsManager()) scene_ptr->getSceneGraphicsManager()->destroySceneNode(node);
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

//-------------------------------------------------------------------------------------
void Item::integratePacket(ItemPacket packet)
{
  setPosition(packet.position);
}

//-------------------------------------------------------------------------------------
ItemPacket Item::extractPacket()
{
  ItemPacket packet = ItemPacket();
  packet.position = getPosition();
  return packet;
}

