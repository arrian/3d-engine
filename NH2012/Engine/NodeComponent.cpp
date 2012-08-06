#include "NodeComponent.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
NodeComponent::NodeComponent(void)
  : node(0)
{
}

//-------------------------------------------------------------------------------------
NodeComponent::~NodeComponent(void)
{
}

//-------------------------------------------------------------------------------------
void NodeComponent::setNode(Scene* scene, Ogre::SceneNode* node)
{
  setScene(scene);//only set scene if it is different to the current scene

  oldNode = this->node;
  this->node = node;

  hasNodeChange();
}

//-------------------------------------------------------------------------------------
void NodeComponent::removeNode()
{
  setNode(0, 0);
}

//-------------------------------------------------------------------------------------
Ogre::SceneNode* NodeComponent::getNode()
{
  return node;
}

//-------------------------------------------------------------------------------------
void NodeComponent::hasSceneChange()
{
  
}
