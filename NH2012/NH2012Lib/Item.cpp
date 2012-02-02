#include "Item.h"


Item::Item(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
           Ogre::Vector3 position, int id)
 : Entity(sceneManager, physics, "item.mesh", position, id, "rock")
{

  //Physics
  physicsShape = new OgreBulletCollisions::BoxCollisionShape(entity->getBoundingBox().getSize()/2);
  physicsBody = new OgreBulletDynamics::RigidBody("entity" + Ogre::StringConverter::toString(id), physics);
  physicsBody->setShape(node, physicsShape, 0.2f, 0.5f, 0.1f, position);
}


Item::~Item(void)
{
  if(physicsBody) delete physicsBody;
  physicsBody = 0;
  if(physicsShape) delete physicsShape;
  physicsShape = 0;
}
