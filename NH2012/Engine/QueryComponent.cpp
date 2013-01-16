#include "QueryComponent.h"

#include "Scene.h"
#include "Item.h"
#include "Interactive.h"
#include "IdentificationInterface.h"


QueryComponent::QueryComponent(void)
  : NodeComponent()
{
}

//-------------------------------------------------------------------------------------
QueryComponent::~QueryComponent(void)
{
}

//-------------------------------------------------------------------------------------
void QueryComponent::update(double elapsedSeconds)
{

}

//-------------------------------------------------------------------------------------
void QueryComponent::hasNodeChange()
{
  if(!scene || !node) return;

  //debugHitPosition = scene->getGraphicsManager()->createEntity("1mBox.mesh");
  //hitNode = node->createChildSceneNode();
  //hitNode->attachObject(debugHitPosition);
  

}

//-------------------------------------------------------------------------------------
IdentificationInterface* QueryComponent::rayQuery(Vector3 position, Vector3 direction, float distance, Group groups)
{
  Vector3 unitDirection = direction.normalisedCopy();
  physx::PxSceneQueryFlags outputFlags = physx::PxSceneQueryFlags();
  physx::PxSceneQueryFilterData filterData = physx::PxSceneQueryFilterData();
  filterData.data.word0 = groups;
  physx::PxRaycastHit hit;
  if(scene->getPhysicsManager()->raycastSingle(physx::PxVec3(position.x, position.y, position.z), physx::PxVec3(unitDirection.x, unitDirection.y, unitDirection.z), distance, outputFlags, hit, filterData))
  {
    //hitNode->_setDerivedPosition(Vector3(hit.impact.x, hit.impact.y, hit.impact.z));
    
    if(hit.shape->getActor().userData)
    {      
      IdentificationInterface* target = static_cast<IdentificationInterface*>(hit.shape->getActor().userData);//Unsafe operation. Have to make certain that the void pointer has been cast to PhysicalInterface initially

      return target;
    }
  }
  return NULL;
}

//-------------------------------------------------------------------------------------
IdentificationInterface* QueryComponent::rayQuery(Vector3 direction, float distance, Group groups)
{
  return rayQuery(node->_getDerivedPosition(), direction, distance, groups);
}

//-------------------------------------------------------------------------------------
bool QueryComponent::sweepQuery(Vector3 direction, float distance)
{
  return false;
}

//-------------------------------------------------------------------------------------
bool QueryComponent::overlapQuery()
{
  return false;
}

