#pragma once

#include "Interactive.h"

class Door : public Interactive
{
public:
  Door(void);
  virtual ~Door(void);

  void setOpen(bool open);
  void setLocked(bool locked);

  bool isOpen();
  bool isLocked();

  virtual void interact();
  virtual void update(double elapsedSeconds);

protected:
  virtual void hasSceneChange();

private:
  bool open;
  bool locked;

  Ogre::Entity* frame;
  Ogre::Entity* door;

  Ogre::SceneNode* node;
  Ogre::Vector3 position;
  Ogre::Quaternion rotation;

};

