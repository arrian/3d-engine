#pragma once

#include "OIS.h"

#include "ControlMap.h"

class Button
{
public:
  Button()
  {
    hasKeyboardButton = false;
    hasMouseButton = false;
  }

  Button(std::string buttonAsString)
  {
    if(ControlMap::getInstance().isKeyboardButtonString(buttonAsString))
    {
      keyboardButton = ControlMap::getInstance().getAsKeyboardButton(buttonAsString);
      hasKeyboardButton = true;
      hasMouseButton = false;
    }
    else if(ControlMap::getInstance().isMouseButtonString(buttonAsString))
    {
      mouseButton = ControlMap::getInstance().getAsMouseButton(buttonAsString);
      hasKeyboardButton = false;
      hasMouseButton = true;
    }
    else throw NHException("could not convert an input string to a button");
  }

  Button(OIS::KeyCode button)
  {
    keyboardButton = button;
    hasKeyboardButton = true;
    hasMouseButton = false;
  }

  Button(OIS::MouseButtonID button)
  {
    mouseButton = button;
    hasKeyboardButton = false;
    hasMouseButton = true;
  }

  virtual ~Button() {}

  bool operator==(const Button &other) const
  {
    if(other.hasKeyboardButton && this->hasKeyboardButton) return other.keyboardButton == this->keyboardButton;
    if(other.hasMouseButton && this->hasMouseButton) return other.mouseButton == this->mouseButton;
    return false;
  }

  bool isKeyboardButton(OIS::KeyCode button)
  {
    return hasKeyboardButton && this->keyboardButton == button;
  }

  bool isMouseButton(OIS::MouseButtonID button)
  {
    return hasMouseButton && this->mouseButton == button;
  }

  std::string getAsString()
  {
    if(hasKeyboardButton) return ControlMap::getInstance().getAsString(keyboardButton, false);
    else if(hasMouseButton) return ControlMap::getInstance().getAsString(mouseButton);
    return "";
  }

private:
  bool hasKeyboardButton;
  OIS::KeyCode keyboardButton;

  bool hasMouseButton;
  OIS::MouseButtonID mouseButton;
};

