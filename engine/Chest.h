#pragma once

#include "Interactive.h"

#include <OgreSceneNode.h>

class Chest : public Interactive
{
public:
  Chest(void);
  ~Chest(void);

  void setOpen(bool open);
  void setLocked(bool locked);

  bool isOpen();
  bool isLocked();

  virtual void interact();
  virtual void update(double elapsedSeconds);

  virtual void setPosition(Vector3 position);
  virtual void setRotation(Quaternion rotation);

  virtual Vector3 getPosition();
  virtual Quaternion getRotation();

protected:
  void hasSceneChange();

private:
  bool open;
  bool locked;

  Ogre::SceneNode* node;
};

