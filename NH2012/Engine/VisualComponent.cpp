#include "VisualComponent.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
VisualComponent::VisualComponent(Ogre::String mesh)
  : NodeComponent(),
    entity(NULL),
    mesh(mesh)
{
  
}

//-------------------------------------------------------------------------------------
VisualComponent::~VisualComponent(void)
{
}

//-------------------------------------------------------------------------------------
void VisualComponent::update(double elapsedSeconds)
{

}

//-------------------------------------------------------------------------------------
void VisualComponent::hasNodeChange()
{
  if(oldScene && entity) oldScene->getSceneManager()->destroyEntity(entity);
  entity = NULL;

  entity = scene->getSceneManager()->createEntity(mesh);
  node->attachObject(entity);

  node->setVisible(true);
}

//-------------------------------------------------------------------------------------
Ogre::Entity* VisualComponent::getEntity()
{

  return entity;
}