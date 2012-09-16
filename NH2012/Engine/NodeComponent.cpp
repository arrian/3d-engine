#include "NodeComponent.h"

#include "Scene.h"

//-------------------------------------------------------------------------------------
NodeComponent::NodeComponent(void)
  : node(NULL)
{
}

//-------------------------------------------------------------------------------------
NodeComponent::~NodeComponent(void)
{
}

//-------------------------------------------------------------------------------------
void NodeComponent::setNode(Scene* scene, Ogre::SceneNode* node)
{
  setScene(scene);

  oldNode = this->node;
  this->node = node;

  hasNodeChange();
}

//-------------------------------------------------------------------------------------
void NodeComponent::removeNode()
{
  setNode(NULL, NULL);
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
