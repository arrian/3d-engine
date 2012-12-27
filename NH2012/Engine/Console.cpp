#include "Console.h"

#include "Scene.h"
#include "Player.h"
#include "World.h"
#include "ScriptManager.h"
#include "KeyboardMap.h"

#include <boost/algorithm/string.hpp> 


//-------------------------------------------------------------------------------------
Console::Console()
  : overlay(new Gorilla::Silverback),
    isShift(false),
    isControl(false),
    window(NULL),
    screen(NULL),
    layer(NULL),
    view(NULL),
    command(""),
    history(),
    lines(),
    displayLines(27),
    displayOffset(0),
    historyIndex(0),
    firstKeyPressHoldWait(1.0),
    consecutiveKeyPressHoldWait(0.1),
    holdAccumulator(0.0),
    cursorFlashRate(1.0),
    cursorAccumulator(0.0),
    showCursor(true),
    hasDoneFirstHold(false),
    previousKey(OIS::KC_UNASSIGNED),
    keyIsDown(false)

{
  overlay->loadAtlas("dejavu");
}

//-------------------------------------------------------------------------------------
Console::~Console(void)
{
}

//-------------------------------------------------------------------------------------
void Console::hookWindow(Ogre::RenderWindow* window)
{
  assert(window);
  bool visibleTemp = isVisible();
  //if(screen) overlay->destroyScreen(screen);//might be necessary for scene changes or memory leaks?
  this->window = window;
  screen = overlay->createScreen(window->getViewport(0), "dejavu");
  layer = screen->createLayer();
  view = layer->createMarkupText(9, 0, 0, "");

  update();
  setVisible(visibleTemp);
}

//-------------------------------------------------------------------------------------
void Console::rehookWindow()
{
  hookWindow(world->getPlayer()->getWindow());
}

//-------------------------------------------------------------------------------------
void Console::setVisible(bool visible)
{
  if(layer) layer->setVisible(visible);
}

//-------------------------------------------------------------------------------------
bool Console::isVisible()
{
  if(layer) return layer->isVisible();
  return false;
}

//-------------------------------------------------------------------------------------
void Console::clear()
{
  displayOffset = 0;
  lines.empty();
  command = "";
}

//-------------------------------------------------------------------------------------
void Console::update(double elapsedSeconds)
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
void Console::keyPressed(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;

  if(arg.key == OIS::KC_SPACE) command += ' ';
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
  else if(arg.key == OIS::KC_5 && isShift) command += "%%";//single percent used as escape sequence in gorilla markup
  else if(arg.key == OIS::KC_UP && isShift) displayOffset++;//show previous text
  else if(arg.key == OIS::KC_DOWN && isShift) displayOffset--;//show more recent text
  else if(arg.key == OIS::KC_UP) up();
  else if(arg.key == OIS::KC_DOWN) down();
  else 
  {
    try
    {
      command += KeyboardMap::getInstance().getAsString(arg.key, isShift);
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
void Console::keyReleased(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;
  if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) isShift = false;
  else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) isControl = false;
  else keyIsDown = false;
}

//-------------------------------------------------------------------------------------
void Console::enter()
{
  historyIndex = 0;
  history.push_back(command);
  lines.push_back("> " + command);

  try
  {
    world->getScriptManager()->execute(command);
  }
  catch(NHException e)
  {
    error(e.what());
  }

  command = "";
}

//-------------------------------------------------------------------------------------
void Console::print(std::string highlight, std::string comment)
{
  display(highlight, comment);
  update();
}

//-------------------------------------------------------------------------------------
void Console::print(std::string comment)
{
  display(comment);
  update();
}

//-------------------------------------------------------------------------------------
void Console::display(std::string comment)
{
  lines.push_back(comment);
}

//-------------------------------------------------------------------------------------
void Console::display(std::string highlight, std::string comment)
{
  lines.push_back("%5" + highlight + "%r - " + comment);
}

//-------------------------------------------------------------------------------------
void Console::error(std::string comment)
{
  lines.push_back("%4error%r - " + comment);
}

//-------------------------------------------------------------------------------------
void Console::update()
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

  if(end == lines.end()) viewText += "\n%3> " + command;
    
  if(showCursor) viewText = viewText + "|";
  if(view) view->text(viewText);
}

//-------------------------------------------------------------------------------------
void Console::backspace()
{
  if(command.size() == 0) return;
  command = command.substr(0, command.size() - 1);
}

//-------------------------------------------------------------------------------------
void Console::up()
{
  displayOffset = 0;
  if(historyIndex < history.size()) historyIndex++;
  if(historyIndex > 0) command = history[history.size() - historyIndex];
  else command = "";
}

//-------------------------------------------------------------------------------------
void Console::down()
{
  displayOffset = 0;
  if(historyIndex > 0) historyIndex--;
  if(historyIndex > 0) command = history[history.size() - historyIndex];
  else command = "";
}

//-------------------------------------------------------------------------------------
void Console::setWorld(World* world)
{
  this->world = world;
}




