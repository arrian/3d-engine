#include "Portal.h"

//-------------------------------------------------------------------------------------
Portal::Portal(int id, int targetSceneID, int targetPortalID, Vector3 position, Vector3 lookAt)
  : id(id),
    targetSceneID(targetSceneID),
    targetPortalID(targetPortalID),
    position(position),
    lookAt(lookAt),
    loadArea(),
    moveArea()
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
Vector3 Portal::getPosition()
{
  return position;
}

//-------------------------------------------------------------------------------------
Vector3 Portal::getLookAt()
{
  return lookAt;
}

//-------------------------------------------------------------------------------------
int Portal::getID()
{
  return id;
}

//-------------------------------------------------------------------------------------
bool Portal::isLoadRequired(Vector3 observerPosition)
{
  return loadArea.contains(observerPosition);
}
