#include "Portal.h"


Portal::Portal(Scene* scene, int targetID, Ogre::Vector3 position, Ogre::Vector3 targetPosition)
  : scene(scene),
    targetID(targetID),
    target(0),
    door(0),
    targetPosition(targetPosition),
    focalPoint(position)
{
  isInternal = (scene->getSceneID() == targetID);
}


Portal::~Portal(void)
{
}

void Portal::loadTarget()
{
  target = scene->getWorld()->loadScene(scene->getWorld()->getDataManager()->getScene(targetID)->file, FILE_XML);
}

Scene* Portal::getTarget()
{
  return target;
}

void Portal::injectObserver(NodeComponent* component)
{

}

bool Portal::hasLoadedTarget()
{
  return (target != 0 || isInternal);
}