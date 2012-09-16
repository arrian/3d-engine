#pragma once

#include <vector>
#include <OgreSceneNode.h>

class Scene;
class NodeComponent;

class ComponentList
{
public:
  ComponentList(void);
  ~ComponentList(void);

  void push_back(NodeComponent* component);
  void setNode(Scene* scene, Ogre::SceneNode* node);
  
  void update(double elapsedSeconds);
  //void frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
  std::vector<NodeComponent*> components;
};

