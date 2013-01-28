#pragma once


#include <string>

#include "OIS.h"

#include "Screen.h"
#include "ConsoleScreen.h"
#include "MenuScreen.h"
#include "PlayerScreen.h"
#include "LoadingScreen.h"
#include "CursorScreen.h"

class World;

class Interface
{
public:
  Interface(World* world);
  virtual ~Interface(void);

  void update(double elapsedSeconds);
  void hookWindow(Ogre::RenderWindow* window);//need to create new screens and layers
  void resizeWindow(Ogre::Vector2 dimensions);

  //Dialogs
  //void displayMessageDialog(std::string message);
  //void displayMessageDialogCallback(std::string message, void (*okCallback)(void));
  //void displayMessageDialogCallback(std::string message, void (*okCallback)(void), void (*cancelCallback)(void));
  
  //Screens
  void displayMenuScreen();
  void displayPlayerScreen();
  void displayLoadingScreen();
  void displayConsoleScreen();

  //Input
  bool capturedInput();
  void keyPressed(const OIS::KeyEvent &arg);
  void keyReleased(const OIS::KeyEvent &arg);
  void mouseMoved(const OIS::MouseEvent &arg);
  void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

protected:
  void setDisplay(Screen* display);

private:
  World* world;
  Screen* display;//currently displayed screen

  ConsoleScreen consoleScreen;
  MenuScreen menuScreen;
  PlayerScreen playerScreen;
  LoadingScreen loadingScreen;
  CursorScreen cursorScreen;

  Ogre::RenderWindow* window;
  Gorilla::Silverback* overlay;
  Gorilla::Screen* screen;

  Gorilla::Rectangle* cursor;
};

