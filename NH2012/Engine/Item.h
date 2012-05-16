#pragma once

#include <OgreString.h>


#include "Attributes.h"
#include "VisualComponent.h"
#include "PhysicalComponent.h"
#include "BasicComponent.h"

class Scene;

class Item : public BasicComponent
{
public:
  Item();
  virtual ~Item(void);

  friend bool operator==(const Item& x, const Item& y);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void setPosition(Ogre::Vector3 position);

protected:
  Ogre::Vector3 position;

  Ogre::SceneNode* node;

  /*! Gold value of the item.*/
  int value;

  /*! Item name.*/
  Ogre::String name;

  /*! eg. book,weapon etc.
      Maps directly to the items.dat file
  */
  ItemAttribute::Type type;
  
  /*! Item's 'special abilities'.*/
  std::vector<GeneralAttribute::Intrinsics> intrinsics;

  void hasSceneChange();

private:
  VisualComponent visual;
  PhysicalComponent physical;
};

