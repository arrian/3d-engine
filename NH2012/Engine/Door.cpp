#include "Door.h"

#include "Scene.h"
#include "World.h"

Door::Door(void)
  : Interactive("Door"),
    open(false),
    locked(false),
    doorEntity(NULL),
    doorShape(NULL),
    doorActor(NULL),
    hingeJoint(NULL)
{
}

//-------------------------------------------------------------------------------------
Door::~Door(void)
{
}

//-------------------------------------------------------------------------------------
void Door::hasSceneChange()
{

  //remove stuff from old scene here
  if(oldScene && node)
  {
    oldScene->getSceneManager()->destroySceneNode(node);
  }

  if(!scene) return;

  //define constraint positions here
  //create door and frame here

  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();

  doorActor = scene->getPhysicsManager()->getPhysics().createRigidDynamic(physx::PxTransform());
  ArchitectureDesc doorDesc = scene->getWorld()->getDataManager()->getArchitecture(200);//temp constants
  doorEntity = scene->getSceneManager()->createEntity(doorDesc.mesh);
  physx::PxMaterial* doorMaterial = scene->getPhysicsManager()->getPhysics().createMaterial(doorDesc.friction, doorDesc.friction, doorDesc.restitution);
  doorShape = doorActor->createShape(physx::PxConvexMeshGeometry(scene->getWorld()->getFabricationManager()->createConvexMesh(doorEntity->getMesh())), *doorMaterial);

  hingeJoint = physx::PxRevoluteJointCreate(scene->getPhysicsManager()->getPhysics(), doorActor, doorConstraint, NULL, frameConstraint);
}

//-------------------------------------------------------------------------------------
void Door::update(double elapsedSeconds)
{

}

//-------------------------------------------------------------------------------------
void Door::setOpen(bool open)
{
  this->open = open;
}

//-------------------------------------------------------------------------------------
void Door::setLocked(bool locked)
{
  this->locked = locked;
}

//-------------------------------------------------------------------------------------
bool Door::isOpen()
{
  return open;
}

//-------------------------------------------------------------------------------------
bool Door::isLocked()
{
  return locked;
}

//-------------------------------------------------------------------------------------
void Door::interact()
{
  setOpen(!isOpen());
}

//-------------------------------------------------------------------------------------
void Door::setPosition(Vector3 position)
{
  if(node) node->setPosition(position);
  else throw NHException("door must be inside a scene to set its position");
}

//-------------------------------------------------------------------------------------
void Door::setRotation(Quaternion rotation)
{
  if(node) node->setOrientation(rotation);
  else throw NHException("door must be inside a scene to set its rotation");
}

//-------------------------------------------------------------------------------------
Vector3 Door::getPosition()
{
  if(node) return node->getPosition();
  throw NHException("door must be inside a scene to get its position");
}

//-------------------------------------------------------------------------------------
Quaternion Door::getRotation()
{
  if(node) return node->getOrientation();
  throw NHException("door must be inside a scene to get its rotation");
}