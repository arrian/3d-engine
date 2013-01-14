#pragma once

#include <string>
#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include "NodeComponent.h"

class MeshComponent : public NodeComponent
{
public:
  MeshComponent(std::string mesh);
  ~MeshComponent(void);

  void update(double elapsedSeconds);

  Ogre::Entity* getEntity();

  void setMesh(std::string mesh);

protected:
  void hasNodeChange();

private:
  std::string mesh;
  Ogre::Entity* entity;

  void updateEntity();
};

