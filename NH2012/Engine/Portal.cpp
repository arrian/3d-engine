#include "Portal.h"

//-------------------------------------------------------------------------------------
Portal::Portal(int id, int targetSceneID, int targetPortalID, Ogre::Vector3 position, Ogre::Vector3 lookAt)
  : id(id),
    targetSceneID(targetSceneID),
    targetPortalID(targetPortalID),
    position(position),
    lookAt(lookAt)
{
}

//-------------------------------------------------------------------------------------
Portal::~Portal(void)
{
}

//-------------------------------------------------------------------------------------
int Portal::getTargetScene()
{
  return targetSceneID;
}

//-------------------------------------------------------------------------------------
int Portal::getTargetPortal()
{
  return targetPortalID;
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Portal::getPosition()
{
  return position;
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Portal::getLookAt()
{
  return lookAt;
}

//-------------------------------------------------------------------------------------
int Portal::getID()
{
  return id;
}


