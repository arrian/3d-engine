#pragma once

#include "World.h"

class ScriptManager
{
public:
  ScriptManager(void);
  virtual ~ScriptManager(void);

  void setWorld(World* world);

private:
  World* world;
};

