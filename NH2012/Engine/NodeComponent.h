#pragma once

#include <OgreSceneNode.h>
#include "BasicComponent.h"
#include "Vector3.h"

class Scene;

class NodeComponent : public BasicComponent
{
public:
  NodeComponent(void);
  virtual ~NodeComponent(void);

  virtual void update(double elapsedSeconds) = 0;

  void setNode(boost::shared_ptr<Scene> scene, Ogre::SceneNode* node);//temporary requires scene as confirmation
  void removeNode();

  Ogre::SceneNode* getNode();
protected:
  Ogre::SceneNode* node;
  Ogre::SceneNode* oldNode;

  void hasSceneChange();

  /* Called when new node set or new scene set. Note that either node may not exist.*/
  virtual void hasNodeChange() = 0;
};

