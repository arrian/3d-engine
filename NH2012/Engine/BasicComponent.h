#pragma once

#include "Vector3.h"

class Scene;

class BasicComponent
{
public:
  BasicComponent(void);
  virtual ~BasicComponent(void);

  virtual void update(double elapsedSeconds) = 0;
    
  void setScene(Scene* scene);

  Scene* getScene();

protected:
  Scene* scene;
  Scene* oldScene;

  virtual void hasSceneChange() = 0;
};

