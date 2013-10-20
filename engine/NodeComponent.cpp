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
void NodeComponent::setNode(boost::shared_ptr<Scene> scene, Ogre::SceneNode* node)
{
  setScene(scene);

  oldNode = this->node;
  this->node = node;

  hasNodeChange();
}

//-------------------------------------------------------------------------------------
void NodeComponent::removeNode()
{
  setNode(boost::shared_ptr<Scene>(), NULL);
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
