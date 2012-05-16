#include "Portal.h"


Portal::Portal(Scene* scene, int targetID, Ogre::Vector3 position)
  : scene(scene),
    targetID(targetID),
    target(0)
{
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
