#pragma once

//Standard
#include <vector>

//Local
#include "Button.h"

//Keyboard and Mouse Controls
class ControlManager
{
public:
  ControlManager() {}
  virtual ~ControlManager() {}

  //Move
  Button moveForward;
  Button moveLeft;
  Button moveBack;
  Button moveRight;

  //Actions
  Button jump;
  Button run;
  Button crouch;

  //Hands
  Button leftHand;
  Button rightHand;
  Button interact;

  //Miscellaneous
  std::vector<Button> quickslots;
  Button exit;

  //Debug
  Button addItem;
  Button addCreature;
  Button console;
};
