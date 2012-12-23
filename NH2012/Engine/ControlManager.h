
#pragma once

#include <map>
#include <boost/assign/list_of.hpp>

//OIS
#include <OISKeyboard.h>
#include <OISMouse.h>

#include "NHException.h"

//Keyboard and Mouse Controls
class ControlManager
{
public:
  ControlManager();
  virtual ~ControlManager();

  OIS::KeyCode stringToKeyCode(std::string string);

  //Move
  OIS::KeyCode moveForward;
  OIS::KeyCode moveLeft;
  OIS::KeyCode moveBack;
  OIS::KeyCode moveRight;

  //Actions
  OIS::KeyCode jump;
  OIS::KeyCode run;
  OIS::KeyCode crouch;

  //Hands
  OIS::MouseButtonID leftHand;
  OIS::MouseButtonID rightHand;
  OIS::KeyCode interact;

  //Miscellaneous
  std::vector<OIS::KeyCode> quickslots;
  OIS::KeyCode exit;

  //Debug
  OIS::KeyCode addItem;
  OIS::KeyCode addMonster;
  OIS::KeyCode console;

private:
  std::map<std::string, OIS::KeyCode> stringToEnum;
};
