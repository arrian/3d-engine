#pragma once

#include "Screen.h"

#include "Bar.h"

class LoadingScreen : public Screen
{
public:
  LoadingScreen();
  virtual ~LoadingScreen(void);

private:
  Bar progress;
};

