#include "BasicComponent.h"

#include "PointerUtils.h"

//-------------------------------------------------------------------------------------
BasicComponent::BasicComponent(void)
  : scene(),
    oldScene()
{
}

//-------------------------------------------------------------------------------------
BasicComponent::~BasicComponent(void)
{
}

//-------------------------------------------------------------------------------------
void BasicComponent::setScene(boost::shared_ptr<Scene> scene)
{
  setOldScene(getScene());
  this->scene = getWeakFromShared(scene);
  hasSceneChange();
}

//-------------------------------------------------------------------------------------
boost::shared_ptr<Scene> BasicComponent::getScene()
{
  return getSharedFromWeak(scene, "Failed to get scene from basic component. Either the scene never existed or it has expired.");
}

//-------------------------------------------------------------------------------------
boost::shared_ptr<Scene> BasicComponent::getOldScene()
{
  return getSharedFromWeak(oldScene, "Failed to get old scene from basic component. Either the old scene never existed or it has expired.");
}

//-------------------------------------------------------------------------------------
void BasicComponent::setOldScene(boost::shared_ptr<Scene> oldScene)
{
  this->oldScene = getWeakFromShared(oldScene);
}

//-------------------------------------------------------------------------------------
bool BasicComponent::hasScene()
{
  return scene.expired();
}

//-------------------------------------------------------------------------------------
bool BasicComponent::hasOldScene()
{
  return oldScene.expired();
}


