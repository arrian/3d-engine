#pragma once

#include "BasicComponent.h"
#include "Identifiable.h"
#include "DataManager.h"
#include "Physical.h"

class Interactive : public BasicComponent, public Physical, public Identifiable
{
public:
  Interactive(InteractiveDesc desc);
  virtual ~Interactive(void);

  virtual void interact() {}
  virtual void update(double elapsedSeconds) {}

  virtual void setPosition(Vector3 position) {this->position = position;}
  virtual void setRotation(Quaternion rotation) {this->rotation = rotation;}

  virtual Vector3 getPosition() {return position;}
  virtual Quaternion getRotation() {return rotation;}

protected:
  virtual void hasSceneChange() {}

private:
  InteractiveDesc desc;

  Vector3 position;
  Quaternion rotation;
};



