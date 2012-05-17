#include "Item.h"

#include "Scene.h"


Item::Item()
  : BasicComponent(),
    visual("item.mesh"),
    physical(),
    node(0)
{
  setPosition(Ogre::Vector3::ZERO);
  //physicsBody->setShape(node, physicsShape, 0.2f, 0.95f, 1000.0f, position);
}

void Item::hasSceneChange()
{
  if(oldScene && node) oldScene->getSceneManager()->destroySceneNode(node);
  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  visual.setNode(scene, node);
  physical.setNode(scene, node);
  physical.mapPhysical(this);
}

Item::~Item(void)
{
  if(scene && node) scene->getSceneManager()->destroySceneNode(node);
  node = 0;
}

bool operator==(const Item& x, const Item& y)
{
  return x.name == y.name && x.type == y.type && x.value == y.value;
}

void Item::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  physical.frameRenderingQueued(evt);
}

void Item::setPosition(Ogre::Vector3 position)
{
  this->position = position;
  if(node) node->setPosition(position);
}


