#include "BasicComponent.h"


BasicComponent::BasicComponent(void)
  : scene(0),
    oldScene(0)
{
}


BasicComponent::~BasicComponent(void)
{
}

void BasicComponent::setScene(Scene* scene)
{
  oldScene = this->scene;
  this->scene = scene;
  hasSceneChange();
}



