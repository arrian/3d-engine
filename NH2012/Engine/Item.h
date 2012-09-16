#pragma once

#include <OgreString.h>

#include "PhysicalInterface.h"
#include "VisualComponent.h"
#include "PhysicalComponent.h"
#include "BasicComponent.h"
#include "DataManager.h"

class Scene;

class Item : public BasicComponent, public PhysicalInterface
{
public:
  //Item(int id);
  Item(ItemDesc description);
  virtual ~Item(void);

  friend bool operator==(const Item& x, const Item& y);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void setPosition(Ogre::Vector3 position);
  void setRotation(Ogre::Quaternion rotation);

  Ogre::Vector3 getPosition();
  Ogre::Quaternion getRotation();

protected:
  Ogre::SceneNode* node;
  Ogre::Vector3 position;
  Ogre::Quaternion rotation;

  void hasSceneChange();

  VisualComponent visual;
  PhysicalComponent physical;
};

