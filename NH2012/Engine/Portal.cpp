#include "Portal.h"

//-------------------------------------------------------------------------------------
Portal::Portal(Id<Scene> targetSceneID, Id<Portal> targetPortalID, Vector3 position, Vector3 lookAt)
  : targetSceneId(targetSceneId),
    targetPortalId(targetPortalId),
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
Id<Scene> Portal::getTargetScene()
{
  return targetSceneId;
}

//-------------------------------------------------------------------------------------
Id<Portal> Portal::getTargetPortal()
{
  return targetPortalId;
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
bool Portal::isLoadRequired(Vector3 observerPosition)
{
  return loadArea.contains(observerPosition);
}
