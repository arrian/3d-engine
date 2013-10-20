#include "Door.h"

#include "Scene.h"
#include "World.h"

#include "ScenePhysicsManager.h"
#include "SceneGraphicsManager.h"

Door::Door(void)
  : Interactive(InteractiveDesc(0, "Door", "invalid.script")),
    open(false),
    locked(false),
    doorPhysical(INTERACTIVE),
    doorMesh(),
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

  boost::shared_ptr<Scene> oldScene_ptr = getOldScene();

  //remove stuff from old scene here
  if(oldScene_ptr && node)
  {
    oldScene_ptr->getSceneGraphicsManager()->destroySceneNode(node);
  }

  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(!scene_ptr) return;

  node = scene_ptr->getSceneGraphicsManager()->createSceneNode();

  ArchitectureDesc doorDesc(scene_ptr->getWorld()->getDataManager()->get<ArchitectureDesc>(200));//temp constants
  doorMesh.setMesh(doorDesc.mesh);
  physx::PxMaterial* doorMaterial = scene_ptr->getScenePhysicsManager()->getScenePhysics()->getPhysics().createMaterial(doorDesc.friction, doorDesc.friction, doorDesc.restitution);
  doorPhysical.begin();
  doorPhysical.addBoxMesh(1.5f, 3.0f, 0.2f, doorMaterial, Vector3(0.75f, 1.5f, 0.0f));
  doorPhysical.end();

  doorPhysical.setNode(scene_ptr, node);
  doorMesh.setNode(scene_ptr, node);

  doorConstraint = physx::PxTransform(physx::PxVec3(0.0f, 1.5f, 0.0f));//, physx::PxQuat(physx::PxPi / 2, physx::PxVec3(-1.0f,0.0f,0.0f)));
  frameConstraint = physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f));
  hingeJoint = physx::PxRevoluteJointCreate(scene_ptr->getScenePhysicsManager()->getScenePhysics()->getPhysics(), doorPhysical.getActor(), doorConstraint, NULL, frameConstraint);
  hingeJoint->setLimit(physx::PxJointLimitPair(-physx::PxPi / 4, physx::PxPi / 4, 0.01f));
  hingeJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eLIMIT_ENABLED, true);
  hingeJoint->setRevoluteJointFlag(physx::PxRevoluteJointFlag::eDRIVE_FREESPIN, true);

}

//-------------------------------------------------------------------------------------
void Door::update(double elapsedSeconds)
{
  doorPhysical.update(elapsedSeconds);
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