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

  //debugHitPosition = scene->getSceneManager()->createEntity("1mBox.mesh");
  //hitNode = node->createChildSceneNode();
  //hitNode->attachObject(debugHitPosition);
  

}

//-------------------------------------------------------------------------------------
IdentificationInterface* QueryComponent::rayQuery(Ogre::Vector3 direction, float distance, Group groups)
{
  Ogre::Vector3 position = node->_getDerivedPosition();
  Ogre::Vector3 unitDirection = direction.normalisedCopy();
  physx::PxSceneQueryFlags outputFlags = physx::PxSceneQueryFlags();
  physx::PxSceneQueryFilterData filterData = physx::PxSceneQueryFilterData();
  filterData.data.word0 = groups;
  physx::PxRaycastHit hit;
  if(scene->getPhysicsManager()->raycastSingle(physx::PxVec3(position.x, position.y, position.z), physx::PxVec3(unitDirection.x, unitDirection.y, unitDirection.z), distance, outputFlags, hit, filterData))
  {
    //hitNode->_setDerivedPosition(Ogre::Vector3(hit.impact.x, hit.impact.y, hit.impact.z));
    
    if(hit.shape->getActor().userData)
    {      
      IdentificationInterface* target = static_cast<IdentificationInterface*>(hit.shape->getActor().userData);//Unsafe operation. Have to make certain that the void pointer has been cast to PhysicalInterface initially

      return target;
    }
  }
  return NULL;
}

//-------------------------------------------------------------------------------------
bool QueryComponent::sweepQuery(Ogre::Vector3 direction, float distance)
{
  return false;
}

//-------------------------------------------------------------------------------------
bool QueryComponent::overlapQuery()
{
  return false;
}

