#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <OISKeyboard.h>
#include <OgreString.h>

#include "SceneChangeListener.h"
#include "Gorilla.h"

class World;


/**
 * In-game command console singleton.
 */
class Console : public SceneChangeListener
{
public:
  static Console& getInstance()
  {
      static Console instance;
      return instance;
  }

  void setWorld(World* world);
  void setKeyboard(OIS::Keyboard* keyboard);

  void hookWindow(Ogre::RenderWindow* window);//hooks the console to a render window
  void rehookWindow();
  bool isVisible();//returns the visibility of the console
  void setVisible(bool visible);//sets the visibility of the console
  void update(double elapsedSeconds);
  void injectKeyDown(const OIS::KeyEvent &arg);//key down
  void injectKeyUp(const OIS::KeyEvent &arg);//key up
  void injectKey(const OIS::KeyEvent &arg, bool isDown);//key down or up
  void keyPressed(const OIS::KeyEvent &arg);

  void print(std::string highlight, std::string comment);
  void print(std::string comment);
  void clear();

  void sceneChanged();
  
private:
  Console();//World* world, OIS::Keyboard* keyboard);
  ~Console(void);
  Console(Console const&);        // Don't Implement
  void operator=(Console const&); // Don't implement

  bool isShift;//True if shift is pressed.
  bool isControl;//True if control is pressed.
  bool showCursor;
  Ogre::Root* root;//Game root.
  Ogre::RenderWindow* window;//Game render window.
  OIS::Keyboard* keyboard;//Keyboard manager.
  World* world;//Game world.
  Gorilla::Silverback* overlay;//Console renderer.
  Gorilla::Screen* screen;//Console screen.
  Gorilla::Layer* layer;//Console layer.
  Gorilla::MarkupText* view;//Console display text.
  std::vector<std::string> lines;
  int displayLines;//number of lines to display at a time
  int displayOffset;
  std::string command;//Current console command.
  std::vector<std::string> history;//The command execution history.
  int historyIndex;

  bool keyIsDown;
  bool hasDoneFirstHold;//has only done the initial key press and no repeats
  double firstKeyPressHoldWait;//how long to wait after the first key press to repeat the key
  double consecutiveKeyPressHoldWait;//how long to wait before a key press is repeated
  double holdAccumulator;
  double cursorFlashRate;
  double cursorAccumulator;
  OIS::KeyCode previousKey;

  void enter();//Submit the command for processing.
  void backspace();//Backspace pressed.
  void update();//Updates the console display.  
  void up();//up pressed
  void down();//down pressed

  void display(std::string comment);//displays a line of text
  void display(std::string highlight, std::string comment);//displays a highlighted section of text followed by a normal section
  void error(std::string comment);//displays an error on the console
};

