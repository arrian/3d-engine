#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <OISKeyboard.h>
#include <OgreString.h>

#include "World.h"
#include "Environment.h"
#include "Gorilla.h"

//TODO Get previous commands when up/down arrow pressed
//TODO Cursor movement, cursor blinking and text selection
//TODO Make console scrollable and ensure console doesn't overflow viewport (vertical and horizontal)
//TODO Console can't handle viewport context switching (ie. level change/sceneManager change)

/*!
 * In-game command console.
 **/
class Console
{
public:
  Console(Ogre::Root* root, Ogre::RenderWindow* window, OIS::Keyboard* keyboard, World* world);
  ~Console(void);

  /*!
  * Returns the visibility of the console.
  **/
  bool isVisible();

  /*!
  * Sets console visibility.
  **/
  void setVisible(bool visible);

  /*!
  * Frame rendering is queued.
  **/
  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  /*!
  * Key pressed.
  **/
  void injectKeyDown(const OIS::KeyEvent &arg);

  /*!
  * Key released.
  **/
  void injectKeyUp(const OIS::KeyEvent &arg);

  /*!
  * Displays a simple line of text.
  **/
  void display(Ogre::String comment);

  /*!
  * Displays a highlighted section of text followed by normal text.
  **/
  void display(Ogre::String highlight, Ogre::String comment);

  /*!
  * Displays an error on the console.
  **/
  void error(Ogre::String comment);

private:
  /*!
  * True if shift is pressed.
  **/
  bool isShift;

  /*!
  * True if control is pressed.
  **/
  bool isControl;

  /*!
  * Game root.
  **/
  Ogre::Root* root;

  /*!
  * Game render window.
  **/
  Ogre::RenderWindow* window;

  /*!
  * Keyboard manager.
  **/
  OIS::Keyboard* keyboard;

  /*!
  * Game world.
  **/
  World* world;

  /*!
  * Console renderer.
  **/
  Gorilla::Silverback* gui;

  /*!
  * Console screen.
  **/
  Gorilla::Screen* screen;
  
  /*!
  * Console layer.
  **/
  Gorilla::Layer* layer;

  /*!
  * Console display text.
  **/
  Gorilla::MarkupText* view;

  /*!
  * Console internal text.
  **/
  Ogre::String text;

  /*!
  * Current console command.
  **/
  Ogre::String command;

  /*!
  * Submit the command for processing.
  **/
  void enter();

  /*!
  * Backspace pressed.
  **/
  void backspace();

  /*!
  * Display help.
  **/
  void help();
  
  /*!
  * Clear the console.
  **/
  void clear();
  
  /*!
  * Display game statistics.
  **/
  void stats();

  /*!
  * Updates the console display.
  **/
  void update();

  /*!
  * Displays a boolean as text on console (enabled/disabled).
  **/
  void display(bool option);

  /*!
  * Tokenises a string by the given delimiter.
  **/
  void split(const std::string &s, char delim, std::vector<std::string> &elems);
};

