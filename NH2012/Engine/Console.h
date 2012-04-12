#pragma once

#include <OgreString.h>

class Console
{
public:
  Console(void);
  ~Console(void);

  void show();
  void hide();
  bool isVisible();

  /*! Injects a debug command.*/
  void injectCharacter(Ogre::String command);
  
  void injectEnter();

  void refresh();

private:
  bool visible;

  Ogre::String command;
  Ogre::String trace;

  void help();
  void clear();
};

