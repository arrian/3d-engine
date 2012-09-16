#include "ComponentList.h"

#include "NodeComponent.h"
#include "Scene.h"

//-------------------------------------------------------------------------------------
ComponentList::ComponentList(void)
  : components()
{
}

//-------------------------------------------------------------------------------------
ComponentList::~ComponentList(void)
{
}

//-------------------------------------------------------------------------------------
void ComponentList::push_back(NodeComponent* component)
{
  components.push_back(component);
}

//-------------------------------------------------------------------------------------
void ComponentList::setNode(Scene* scene, Ogre::SceneNode* node)
{
  for(std::vector<NodeComponent*>::iterator iter = components.begin(); iter < components.end(); ++iter) (*iter)->setNode(scene, node);
}

//-------------------------------------------------------------------------------------
void ComponentList::update(double elapsedSeconds)
{
  for(std::vector<NodeComponent*>::iterator iter = components.begin(); iter < components.end(); ++iter) (*iter)->update(elapsedSeconds);
}




