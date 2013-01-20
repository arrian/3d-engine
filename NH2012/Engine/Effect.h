#pragma once

#include "BasicComponent.h"

class Effect : public BasicComponent
{
public:
  Effect(void);
  virtual ~Effect(void);

  void update(double elapsedSeconds);

protected:
  void hasSceneChange();

};

