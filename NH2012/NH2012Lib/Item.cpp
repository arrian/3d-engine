#include "Item.h"


Item::Item(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, Ogre::Vector3 position, int id) : Entity(sceneManager, physics, "item.mesh", position, id)
{
  value = 0;
  name = "rock";

  attributes = ItemAttributes();

  attributes.buc = ItemAttribute::UNCURSED;
  attributes.type = ItemAttribute::GEM;
}


Item::~Item(void)
{
}

bool operator==(const Item& x, const Item& y)
{
  return (x.name == y.name);//should also check if all attributes are identical
}