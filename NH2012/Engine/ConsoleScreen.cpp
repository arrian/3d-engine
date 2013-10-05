#include "ConsoleScreen.h"

#include <boost/algorithm/string.hpp> 

#include "Gorilla.h"
#include "World.h"
#include "Scene.h"
#include "Player.h"
#include "ScriptManager.h"
#include "ControlMap.h"

ConsoleScreen::ConsoleScreen(ScriptManager* scriptManager)
  : Screen(),
    scriptManager(scriptManager),
    isShift(false),
    isControl(false),
    showCursor(true),
    hasDoneFirstHold(false),
    keyIsDown(false),
    view(NULL),
    command(""),
    history(),
    lines(),
    displayLines(27),
    displayOffset(0),
    historyIndex(0),
    cursorIndex(0),
    firstKeyPressHoldWait(1.0),
    consecutiveKeyPressHoldWait(0.1),
    holdAccumulator(0.0),
    cursorFlashRate(1.0),
    cursorAccumulator(0.0),
    previousKey(OIS::KC_UNASSIGNED)
{
  scriptManager->addOutputTarget(this);
  scriptManager->setConsole(this);
}

//-------------------------------------------------------------------------------------
ConsoleScreen::~ConsoleScreen(void)
{
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::setScriptManager(ScriptManager* scriptManager)
{
  this->scriptManager = scriptManager;
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::hasLayerChange()
{
  view = layer->createMarkupText(9, 0, 0, "");
  update();
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::clear()
{
  displayOffset = 0;
  lines.empty();
  command = "";
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::update(double elapsedSeconds)
{
  if(!isVisible()) return;

  cursorAccumulator += elapsedSeconds;

  if(cursorAccumulator >= cursorFlashRate) 
  {
    showCursor = !showCursor;
    cursorAccumulator = 0.0;
    update();
  }

  if(keyIsDown)
  {
    holdAccumulator += elapsedSeconds;
    if(hasDoneFirstHold)
    {
      if(holdAccumulator >= consecutiveKeyPressHoldWait) 
      {
        keyPressed(OIS::KeyEvent(NULL,previousKey,0));
        holdAccumulator = 0.0;
      }
    }
    else
    {
      if(holdAccumulator >= firstKeyPressHoldWait) 
      {
        keyPressed(OIS::KeyEvent(NULL,previousKey,0));
        holdAccumulator = 0.0;
        hasDoneFirstHold = true;
      }
    }
  }
  else
  {
    holdAccumulator = 0.0;
    hasDoneFirstHold = false;
  }
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::keyPressed(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;

  if(arg.key == OIS::KC_SPACE) addCommandText(cursorIndex, " ");
  else if(arg.key == OIS::KC_GRAVE) return;
  else if(arg.key == OIS::KC_BACK) backspace();
  else if(arg.key == OIS::KC_RETURN) enter();
  else if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) 
  {
    isShift = true; 
    return;
  }
  else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) 
  {
    isControl = true; 
    return;
  }
  else if(arg.key == OIS::KC_5 && isShift) addCommandText(cursorIndex, "%%");//single percent used as escape sequence in gorilla markup
  else if(arg.key == OIS::KC_UP && isShift) displayOffset++;//show previous text
  else if(arg.key == OIS::KC_DOWN && isShift) displayOffset--;//show more recent text
  else if(arg.key == OIS::KC_PGUP) displayOffset++;
  else if(arg.key == OIS::KC_PGDOWN) displayOffset--;
  else if(arg.key == OIS::KC_UP) up();
  else if(arg.key == OIS::KC_DOWN) down();
  else if(arg.key == OIS::KC_LEFT) left();
  else if(arg.key == OIS::KC_RIGHT) right();
  else if(arg.key == OIS::KC_TAB) tab();
  else if(arg.key == OIS::KC_DELETE) del();
  else if(arg.key == OIS::KC_HOME) cursorIndex = 0;
  else if(arg.key == OIS::KC_END) cursorIndex = command.size();
  else 
  {
    try
    {
      addCommandText(cursorIndex, ControlMap::getInstance().getAsString(arg.key, isShift));
    }
    catch(NHException e)
    {
      //unknown key was pressed. just ignore
    }
  }
  cursorAccumulator = 0.0;//we want to be able to see the cursor while typing
  showCursor = true;

  keyIsDown = true;

  update();
  previousKey = arg.key;
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::keyReleased(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;
  if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) isShift = false;
  else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) isControl = false;
  else keyIsDown = false;
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::enter()
{
  historyIndex = 0;
  history.push_back(command);
  lines.push_back("> " + command);
  cursorIndex = 0;

  try
  {
    scriptManager->execute(command);
  }
  catch(NHException e)
  {
    error(e.what());
  }

  command = "";
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::print(std::string highlight, std::string comment)
{
  display(highlight, comment);
  update();
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::print(std::string comment)
{
  display(comment);
  update();
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::display(std::string comment)
{
  lines.push_back(comment);
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::display(std::string highlight, std::string comment)
{
  lines.push_back("%5" + highlight + "%r - " + comment);
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::error(std::string comment)
{
  lines.push_back("%4error%r - " + comment);
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::update()
{
  std::string viewText = "";
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = lines.end();
  if(displayOffset < 0) displayOffset = 0;//keep offset above the minimum
  if(displayOffset + displayLines > lines.size()) displayOffset = lines.size() - displayLines;//keep offset below the top
  if(lines.size() < displayLines) iter = lines.begin();//fewer lines than the maximum allowable display lines
  else 
  {
    iter = lines.end() - (displayLines + displayOffset);
    end = iter + displayLines;
  }
  for(; iter < end; ++iter)
  {
    viewText += "\n " + *iter;
  }

  if(end == lines.end()) viewText += "\n%3> ";

  if(showCursor) 
  {
    int cursorPosition = cursorIndex;
    if(cursorIndex > command.size()) cursorPosition = command.size();
    std::string preCursor = command.substr(0, cursorPosition);
    std::string postCursor = command.substr(cursorPosition, command.size() - cursorPosition);

    viewText += preCursor + "|" + postCursor;
  }
  else 
  {
    viewText += command;
  }

  if(view) view->text(viewText);
  
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::backspace()
{
  removeCommandText(cursorIndex - 1, 1);
  if(cursorIndex > 0) cursorIndex--;
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::up()
{
  bool cursorAtEnd = (command.size() == cursorIndex);
  displayOffset = 0;
  if(historyIndex < history.size()) historyIndex++;
  if(historyIndex > 0) command = history[history.size() - historyIndex];
  else command = "";

  if(cursorAtEnd || cursorIndex > command.size()) cursorIndex = command.size();
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::down()
{
  bool cursorAtEnd = (command.size() == cursorIndex);
  displayOffset = 0;
  if(historyIndex > 0) historyIndex--;
  if(historyIndex > 0) command = history[history.size() - historyIndex];
  else command = "";

  if(cursorAtEnd || cursorIndex > command.size()) cursorIndex = command.size();
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::del()
{
  removeCommandText(cursorIndex, 1);
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::left()
{
  if(cursorIndex <= 0) return;
  cursorIndex--;
  if(cursorIndex > command.size()) cursorIndex = command.size();
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::right()
{
  if(cursorIndex >= command.size()) return;
  cursorIndex++;
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::tab()
{
  addCommandText(cursorIndex, "  ");
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::addCommandText(int index, std::string text)
{
  if(command.size() == 0) command += text;
  else if(index <= 0) command = text + command;
  else if(index >= command.size()) command += text;
  else command.insert(index, text);

  cursorIndex += text.size();
}

//-------------------------------------------------------------------------------------
void ConsoleScreen::removeCommandText(int index, int count)
{
  if(command.size() <= 0) return;
  if(index >= command.size()) return;
  command = command.erase(index, count);
}


