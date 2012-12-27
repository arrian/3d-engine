#pragma once

#include "BasicComponent.h"
#include "IdentificationInterface.h"

class Interactive : public BasicComponent, public IdentificationInterface
{
public:
  Interactive(std::string name);
  virtual ~Interactive(void);

  virtual void interact() = 0;
  virtual void update(double elapsedSeconds) = 0;

  virtual void setPosition(Ogre::Vector3 position) = 0;
  virtual void setRotation(Ogre::Quaternion rotation) = 0;

  virtual Ogre::Vector3 getPosition() = 0;
  virtual Ogre::Quaternion getRotation() = 0;

protected:
  virtual void hasSceneChange() = 0;
};



