#include "Entity.h"

Entity::Entity(Ogre::SceneManager* sceneManager, Ogre::String meshName, Ogre::Vector3 position, int id, Ogre::String name)
{
  this->id = id;
  this->name = name;

  entity = sceneManager->createEntity(meshName);
  node = sceneManager->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  node->attachObject(entity);
}


Entity::~Entity(void)
{
}
