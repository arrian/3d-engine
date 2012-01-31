#include "Entity.h"

Entity::Entity(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, Ogre::String meshName, Ogre::Vector3 position, int id, Ogre::String name)
{
  this->id = id;
  this->name = name;

  entity = sceneManager->createEntity(meshName);
  node = sceneManager->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  node->attachObject(entity);


  //Physics
  physicsShape = new OgreBulletCollisions::BoxCollisionShape(entity->getBoundingBox().getSize()/2);
  physicsBody = new OgreBulletDynamics::RigidBody("entity" + Ogre::StringConverter::toString(id), physics);
  physicsBody->setShape(node, physicsShape, 0.2, 0.5, 1, position);
  //physicsBody->
}


Entity::~Entity(void)
{
  if(entity) delete entity;
  entity = 0;
  if(physicsBody) delete physicsBody;
  physicsBody = 0;
  if(physicsShape) delete physicsShape;
  physicsShape = 0;
  if(node) delete node;
  node = 0;
}
