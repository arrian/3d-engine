#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <OISKeyboard.h>
#include <OgreString.h>

#include "SceneType.h"
#include "World.h"
#include "Gorilla.h"

//TODO Get previous commands when up/down arrow pressed
//TODO Cursor movement, cursor blinking and text selection
//TODO Make console scrollable and ensure console doesn't overflow viewport (vertical and horizontal)

/*!
 * In-game command console.
 **/
class Console
{
public:
  Console(World* world, OIS::Keyboard* keyboard);
  ~Console(void);

  /* Hooks this console to a render window.*/
  void hookWindow(Ogre::RenderWindow* window);

  /* Returns the visibility of the console.*/
  bool isVisible();

  /* Sets console visibility.*/
  void setVisible(bool visible);

  /* Frame rendering is queued.*/
  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  /* Key pressed.*/
  void injectKeyDown(const OIS::KeyEvent &arg);

  /* Key released.*/
  void injectKeyUp(const OIS::KeyEvent &arg);

  /* Displays a simple line of text.*/
  void display(std::string comment);

  /* Displays a highlighted section of text followed by normal text.*/
  void display(std::string highlight, std::string comment);

  /* Displays an error on the console.*/
  void error(std::string comment);

private:
  /* True if shift is pressed.*/
  bool isShift;

  /* True if control is pressed.*/
  bool isControl;

  /* Game root.*/
  Ogre::Root* root;

  /* Game render window.*/
  Ogre::RenderWindow* window;

  /* Keyboard manager.*/
  OIS::Keyboard* keyboard;

  /* Game world.*/
  World* world;

  /* Console renderer.*/
  Gorilla::Silverback* overlay;

  /* Console screen.*/
  Gorilla::Screen* screen;
  
  /* Console layer.*/
  Gorilla::Layer* layer;

  /* Console display text.*/
  Gorilla::MarkupText* view;

  /* Console internal text.*/
  std::string text;

  /* Current console command.*/
  std::string command;

  /* Submit the command for processing.*/
  void enter();

  /* Backspace pressed.*/
  void backspace();

  /* Display help.*/
  void help();
  
  /* Clear the console.*/
  void clear();
  
  /* Display game statistics.*/
  void stats();

  /* Updates the console display.*/
  void update();

  /* Displays the no command error.*/
  void noCommand(std::string command);

  /* Tokenises a string by the given delimiter.*/
  void split(const std::string &s, char delim, std::vector<std::string> &elems);
};

