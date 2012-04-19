#include "Item.h"


Item::Item(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
           Ogre::Vector3 position, int id)
 : Entity(sceneManager, physics, "item.mesh", position, id, "rock"),
   physicsShape(new OgreBulletCollisions::BoxCollisionShape(entity->getBoundingBox().getSize()/2)),
   physicsBody(new OgreBulletDynamics::RigidBody("item" + Ogre::StringConverter::toString(id), physics))
{
  physicsBody->setShape(node, physicsShape, 0.2f, 0.95f, 1000.0f, position);
}


Item::~Item(void)
{
  if(physicsBody) delete physicsBody;
  physicsBody = 0;
  if(physicsShape) delete physicsShape;
  physicsShape = 0;
}
