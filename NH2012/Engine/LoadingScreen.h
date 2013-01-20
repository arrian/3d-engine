#pragma once

#include "Screen.h"

#include "Bar.h"

class LoadingScreen : public Screen
{
public:
  LoadingScreen(World* world);
  virtual ~LoadingScreen(void);

private:
  Bar progress;
};

