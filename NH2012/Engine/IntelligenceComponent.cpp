#include "IntelligenceComponent.h"

#include "Scene.h"


IntelligenceComponent::IntelligenceComponent(void)
{
}


IntelligenceComponent::~IntelligenceComponent(void)
{
}

void IntelligenceComponent::go(Ogre::Vector3 to)
{
  points.push(node->getPosition());//temporary queue add
}

Ogre::Vector3 IntelligenceComponent::getNextPosition()
{
  Ogre::Vector3 position = points.front();
  points.pop();
  return position;
}

void IntelligenceComponent::hasNodeChange()
{


}

void IntelligenceComponent::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

}

