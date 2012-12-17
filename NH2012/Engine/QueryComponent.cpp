#include "QueryComponent.h"

#include "Scene.h"


QueryComponent::QueryComponent(void)
  : NodeComponent()
{
}


QueryComponent::~QueryComponent(void)
{
}


void QueryComponent::update(double elapsedSeconds)
{

}

void QueryComponent::hasNodeChange()
{
  if(!scene || !node) return;

  debugHitPosition = scene->getSceneManager()->createEntity("1mBox.mesh");
  hitNode = node->createChildSceneNode();
  hitNode->attachObject(debugHitPosition);
  

}

bool QueryComponent::rayQuery(Ogre::Vector3 direction, float distance)
{

  Ogre::Vector3 position = node->_getDerivedPosition();
  Ogre::Vector3 unitDirection = direction.normalisedCopy();
  physx::PxSceneQueryFlags outputFlags = physx::PxSceneQueryFlags();
  physx::PxRaycastHit hit;
  if(scene->getPhysicsManager()->raycastSingle(physx::PxVec3(position.x, position.y, position.z), physx::PxVec3(unitDirection.x, unitDirection.y, unitDirection.z), distance, outputFlags, hit))
  {
    //std::cout << "ray hit at a distance of " << hit.distance << " with user data " << hit.shape->userData << std::endl;

    hitNode->_setDerivedPosition(Ogre::Vector3(hit.impact.x, hit.impact.y, hit.impact.z));
    
    if(hit.shape->userData)
    {
      //if(hit.shape->userData == (void*) 0x43435453) return;//the default physx character controller insists on using the userData field for its own purposes putting the value 'CCTS' in.
      
      
      //IdentificationInterface* target = static_cast<IdentificationInterface*>(hit.shape->userData);//Unsafe operation. Have to make certain that the void pointer has been cast to PhysicalInterface initially
      //std::cout << target->getType() << ":" << target->getName() << ":" << target->getInstanceID() << std::endl;

    }
    return true;
  }
  return false;
}

bool QueryComponent::sweepQuery(Ogre::Vector3 direction, float distance)
{
  return false;
}

bool QueryComponent::overlapQuery()
{
  return false;
}

