#include "MeshComponent.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
MeshComponent::MeshComponent(std::string mesh)
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

  updateEntity();
}

//-------------------------------------------------------------------------------------
Ogre::Entity* MeshComponent::getEntity()
{
  return entity;
}

//-------------------------------------------------------------------------------------
void MeshComponent::setMesh(std::string mesh)
{
  this->mesh = mesh;
  if(scene && entity) scene->getSceneManager()->destroyEntity(entity);
  entity = NULL;
  updateEntity();
}

//-------------------------------------------------------------------------------------
void MeshComponent::updateEntity()
{
  if(!scene || !node || mesh == "") return;
  entity = scene->getSceneManager()->createEntity(mesh);
  node->attachObject(entity);
  node->setVisible(true);
}
