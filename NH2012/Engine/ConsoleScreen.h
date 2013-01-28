#pragma once

#include <string>
#include <sstream>
#include <vector>

#include <OISKeyboard.h>
#include <OgreString.h>

#include "Screen.h"
#include "ScriptManager.h"

class World;
class Gorilla::MarkupText;


class ConsoleScreen : public Screen, public OutputCallback
{
public:
  ConsoleScreen(ScriptManager* scriptManager);
  virtual ~ConsoleScreen(void);

  void update(double elapsedSeconds);
  virtual void keyPressed(const OIS::KeyEvent &arg);//key down
  virtual void keyReleased(const OIS::KeyEvent &arg);//key up

  virtual void print(std::string highlight, std::string comment);
  virtual void print(std::string comment);
  void clear();

  void setScriptManager(ScriptManager* scriptManager);

protected:
  virtual void hasLayerChange();

private:
  ScriptManager* scriptManager;
  bool isShift;//True if shift is pressed.
  bool isControl;//True if control is pressed.
  bool showCursor;
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

