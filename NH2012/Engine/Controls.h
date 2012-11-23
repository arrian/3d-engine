
#pragma once

//OIS
#include <OISKeyboard.h>
#include <OISMouse.h>

//Keyboard and Mouse Controls
struct Controls
{
  //Move
  OIS::KeyCode moveForward;
  OIS::KeyCode moveLeft;
  OIS::KeyCode moveBack;
  OIS::KeyCode moveRight;

  //Actions
  OIS::KeyCode jump;
  OIS::KeyCode kick;
  OIS::KeyCode run;
  OIS::KeyCode crouch;

  //Hands
  OIS::MouseButtonID leftHand;
  OIS::MouseButtonID rightHand;

  //Miscellaneous
  std::vector<OIS::KeyCode> quickslots;
  OIS::KeyCode exit;

  //Debug
  OIS::KeyCode freezeCollision;
  OIS::KeyCode addItem;
  OIS::KeyCode addMonster;
  OIS::KeyCode reset;
  OIS::KeyCode console;
};
