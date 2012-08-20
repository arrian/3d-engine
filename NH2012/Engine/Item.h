#pragma once

#include <OgreString.h>


#include "Attributes.h"
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
  Item(ItemDesc* description);
  virtual ~Item(void);

  friend bool operator==(const Item& x, const Item& y);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void setPosition(Ogre::Vector3 position);
  void setRotation(Ogre::Quaternion rotation);

  Ogre::Vector3 getPosition();
  Ogre::Quaternion getRotation();

  int getID();
  std::string getType();

protected:
  int id;
  Ogre::Vector3 position;
  Ogre::Quaternion rotation;

  Ogre::SceneNode* node;

  /*! Gold value of the item.*/
  int value;

  /*! Item name.*/
  std::string name;

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

