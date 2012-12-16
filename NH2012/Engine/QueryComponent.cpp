#include "QueryComponent.h"

#include "Scene.h"


QueryComponent::QueryComponent(void)
  : NodeComponent(),
    debugHit(NULL)
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
  if(debugHit && oldScene) oldScene->getSceneManager()->destroyManualObject(debugHit);//cleanup
  
  debugHit = scene->getSceneManager()->createManualObject();
  scene->getSceneManager()->getRootSceneNode()->attachObject(debugHit);
  debugHit->setDynamic(true);//we will be updating the object for every hit
  debugHit->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_LIST);
  debugHit->position(Ogre::Vector3::ZERO);
  debugHit->position(Ogre::Vector3::ZERO);
  debugHit->end();

  debugHitPosition = scene->getSceneManager()->createEntity("watermelon.mesh");
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
    std::cout << "ray hit at a distance of " << hit.distance << " with user data " << hit.shape->userData << std::endl;
    debugHit->beginUpdate(0);
    debugHit->position(node->_getDerivedPosition());
    debugHit->position(Ogre::Vector3(hit.impact.x, hit.impact.y, hit.impact.z));
    debugHit->end();

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

