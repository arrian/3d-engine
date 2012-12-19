#include "MeshComponent.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
MeshComponent::MeshComponent(Ogre::String mesh)
  : NodeComponent(),
    entity(NULL),
    mesh(mesh)
{
  
}

//-------------------------------------------------------------------------------------
MeshComponent::~MeshComponent(void)
{
}

//-------------------------------------------------------------------------------------
void MeshComponent::update(double elapsedSeconds)
{

}

//-------------------------------------------------------------------------------------
void MeshComponent::hasNodeChange()
{
  if(oldScene && entity) oldScene->getSceneManager()->destroyEntity(entity);
  entity = NULL;

  if(!scene || !node) return;

  entity = scene->getSceneManager()->createEntity(mesh);
  node->attachObject(entity);

  node->setVisible(true);
}

//-------------------------------------------------------------------------------------
Ogre::Entity* MeshComponent::getEntity()
{

  return entity;
}


