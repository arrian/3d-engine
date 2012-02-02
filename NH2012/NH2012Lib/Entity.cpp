#include "Entity.h"

Entity::Entity(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
               Ogre::String meshName, Ogre::Vector3 position, int id, Ogre::String name)
{
  this->id = id;
  this->entityName = entityName;

  this->physics = physics;

  entity = sceneManager->createEntity(meshName);
  node = sceneManager->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  node->attachObject(entity);
}


Entity::~Entity(void)
{
  if(entity) delete entity;
  entity = 0;
  if(node) delete node;
  node = 0;
}
