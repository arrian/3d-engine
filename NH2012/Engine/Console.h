#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <OISKeyboard.h>
#include <OgreString.h>

#include "World.h"
#include "Gorilla.h"

//TODO Get previous commands when up/down arrow pressed
//TODO Cursor movement, cursor blinking and text selection
//TODO Make console scrollable and ensure console doesn't overflow viewport (vertical and horizontal)

/**
 * In-game command console.
 */
class Console
{
public:
  Console(World* world, OIS::Keyboard* keyboard);
  ~Console(void);

  void hookWindow(Ogre::RenderWindow* window);//hooks the console to a render window
  bool isVisible();//returns the visibility of the console
  void setVisible(bool visible);//sets the visibility of the console
  void update(double elapsedSeconds);
  void injectKeyDown(const OIS::KeyEvent &arg);//key pressed
  void injectKeyUp(const OIS::KeyEvent &arg);//key released

  void display(std::string comment);//displays a line of text
  void display(std::string highlight, std::string comment);//displays a highlighted section of text followed by a normal section
  void error(std::string comment);//displays an error on the console

private:
  bool isShift;//True if shift is pressed.
  bool isControl;//True if control is pressed.
  Ogre::Root* root;//Game root.
  Ogre::RenderWindow* window;//Game render window.
  OIS::Keyboard* keyboard;//Keyboard manager.
  World* world;//Game world.
  Gorilla::Silverback* overlay;//Console renderer.
  Gorilla::Screen* screen;//Console screen.
  Gorilla::Layer* layer;//Console layer.
  Gorilla::MarkupText* view;//Console display text.
  std::string text;//Console internal text.
  std::string command;//Current console command.
  std::vector<std::string> history;//The command execution history.

  void enter();//Submit the command for processing.
  void backspace();//Backspace pressed.
  void update();//Updates the console display.
  void noCommand(std::string command);//Displays the no command error.
  void split(const std::string &s, char delim, std::vector<std::string> &elems);//Tokenises a string by the given delimiter.


  //Set of executable commands
  void setPhysicsFrozen(bool isFrozen);
  void setFreeCamera(bool isFree);
  void setPlayerPosition(std::string x, std::string y, std::string z);
  void setPlayerScene(std::string sceneName);
  void setPlayerGravity(std::string gravity);
  void setFullscreen(bool isFullscreen, std::string width, std::string height);
  void setAmbientLight(std::string r, std::string g, std::string b, std::string a);
  void showPlayerPosition();
  void showPhysicsInfo();
  void showSceneInfo();
  void showWorldInfo();
  void showGameInfo();
  void showHelp();
  void showAbout();
  void showData(std::string type, std::string id);
  void add(std::string type, std::string id, std::string x, std::string y, std::string z);
  void loadScene(std::string sceneId);
  void unloadScene(std::string sceneName);
  void screenshot();
  void clear();//Clear the console.

};

