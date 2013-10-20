#include "MeshComponent.h"

#include "Scene.h"

#include "SceneGraphicsManager.h"

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
  boost::shared_ptr<Scene> oldScene_ptr = getOldScene();
  if(oldScene_ptr && entity) oldScene_ptr->getSceneGraphicsManager()->destroyEntity(entity);
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
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(scene_ptr && entity) scene_ptr->getSceneGraphicsManager()->destroyEntity(entity);
  entity = NULL;
  updateEntity();
}

//-------------------------------------------------------------------------------------
void MeshComponent::updateEntity()
{
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(!scene_ptr || !node || mesh == "") return;
  entity = scene_ptr->getSceneGraphicsManager()->createEntity(mesh);
  node->attachObject(entity);
  node->setVisible(true);
}
