#pragma once

#include <OgreVector2.h>
#include "Screen.h"

class CursorScreen : public Screen
{
public:
  CursorScreen();
  virtual ~CursorScreen(void);

  virtual void mouseMoved(const OIS::MouseEvent &arg);

  virtual void hasLayerChange();

private:
  Gorilla::Rectangle* cursorImage;
};

