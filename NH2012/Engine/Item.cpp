#include "Item.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
Item::Item(ItemDesc desc)
  : BasicComponent(),
    PhysicalInterface(0, desc.name, "Item"),
    visual(desc.mesh),
    physical(),
    node(NULL)
{
  setPosition(Ogre::Vector3::ZERO);
  setRotation(Ogre::Quaternion::IDENTITY);
}

//-------------------------------------------------------------------------------------
void Item::hasSceneChange()
{
  if(oldScene && node) oldScene->getSceneManager()->destroySceneNode(node);
  node = scene->getSceneManager()->getRootSceneNode()->createChildSceneNode();
  node->setPosition(position);
  node->setOrientation(rotation);
  visual.setNode(scene, node);
  physical.setNode(scene, node);
  physical.mapPhysical((PhysicalInterface*) this);//ensure physical entity points to this item
}

//-------------------------------------------------------------------------------------
Item::~Item(void)
{
  if(scene && node) scene->getSceneManager()->destroySceneNode(node);
  node = NULL;
}

//-------------------------------------------------------------------------------------
bool operator==(const Item& x, const Item& y)
{
  return x.name == y.name;
}

//-------------------------------------------------------------------------------------
void Item::update(double elapsedSeconds)
{
  physical.update(elapsedSeconds);
}

//-------------------------------------------------------------------------------------
void Item::setPosition(Ogre::Vector3 position)
{
  this->position = position;
  if(node) node->setPosition(position);
}

//-------------------------------------------------------------------------------------
void Item::setRotation(Ogre::Quaternion rotation)
{
  this->rotation = rotation;
  if(node) node->setOrientation(rotation);
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Item::getPosition()
{
  return position;
}

//-------------------------------------------------------------------------------------
Ogre::Quaternion Item::getRotation()
{
  return rotation;
}





