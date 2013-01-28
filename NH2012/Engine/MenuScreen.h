#pragma once

#include "Screen.h"

template<typename CallbackObject>
class InterfaceButton;

class MenuScreen : public Screen
{
public:
  MenuScreen(World* world);
  virtual ~MenuScreen(void);

  virtual void mouseMoved(const OIS::MouseEvent &arg);

protected:
  virtual void hasLayerChange();

private:
  std::vector<InterfaceButton<MenuScreen>*> buttons;

  //Actions
  void start();
  void resume();
  void options();
  void exit();
};

