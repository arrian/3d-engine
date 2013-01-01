#include "BasicComponent.h"

//-------------------------------------------------------------------------------------
BasicComponent::BasicComponent(void)
  : scene(NULL),
    oldScene(NULL)
{
}

//-------------------------------------------------------------------------------------
BasicComponent::~BasicComponent(void)
{
}

//-------------------------------------------------------------------------------------
void BasicComponent::setScene(Scene* scene)
{
  oldScene = this->scene;
  this->scene = scene;
  hasSceneChange();
}

//-------------------------------------------------------------------------------------
Scene* BasicComponent::getScene()
{
  return scene;
}


