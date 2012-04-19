#include "Entity.h"

Entity::Entity(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
               Ogre::String meshName, Ogre::Vector3 position, int id, Ogre::String name)
  : id(id),
    entityName(entityName),
    sceneManager(sceneManager),
    physics(physics),
    entity(sceneManager->createEntity(meshName)),
    node(sceneManager->getRootSceneNode()->createChildSceneNode())
{
  node->setPosition(position);
  node->attachObject(entity);
}


Entity::~Entity(void)
{
  if(entity && sceneManager) sceneManager->destroyEntity(entity);
  entity = 0;
  if(node && sceneManager) sceneManager->destroySceneNode(node);
  node = 0;
  sceneManager = 0;
}
