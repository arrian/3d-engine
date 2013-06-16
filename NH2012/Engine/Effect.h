#pragma once

#include "BasicComponent.h"

#include "Physical.h"

class Effect : public BasicComponent, public Physical
{
public:
  Effect(void);
  virtual ~Effect(void);

  void update(double elapsedSeconds);

protected:
  void hasSceneChange();

};

