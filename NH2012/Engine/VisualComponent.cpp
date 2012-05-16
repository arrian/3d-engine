#include "VisualComponent.h"

#include "Scene.h"

VisualComponent::VisualComponent(Ogre::String mesh)
  : NodeComponent(),
    entity(0),
    mesh(mesh)
{
}


VisualComponent::~VisualComponent(void)
{
}

void VisualComponent::frameRenderingQueued(const Ogre::FrameEvent& evt)
{

}

void VisualComponent::hasNodeChange()
{
  if(oldScene && entity) oldScene->getSceneManager()->destroyEntity(entity);
  entity = 0;

  entity = scene->getSceneManager()->createEntity(mesh);
  node->attachObject(entity);

  node->setVisible(true);
}
