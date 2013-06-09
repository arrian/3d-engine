#pragma once

#include "BasicComponent.h"
#include "Identifiable.h"

class Interactive : public BasicComponent, public Physical, public Identifiable
{
public:
  Interactive(std::string name);
  virtual ~Interactive(void);

  virtual void interact() = 0;
  virtual void update(double elapsedSeconds) = 0;

  virtual void setPosition(Vector3 position) = 0;
  virtual void setRotation(Quaternion rotation) = 0;

  virtual Vector3 getPosition() = 0;
  virtual Quaternion getRotation() = 0;

protected:
  virtual void hasSceneChange() = 0;
};



