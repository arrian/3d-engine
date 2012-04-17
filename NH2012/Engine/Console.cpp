#include "Console.h"

Console::Console(Ogre::Root* root, Ogre::RenderWindow* window, OIS::Keyboard* keyboard, World* world)
  : gui(new Gorilla::Silverback),
    keyboard(keyboard),
    isShift(false),
    isControl(false),
    window(window),
    world(world),
    root(root)
{
  gui->loadAtlas("dejavu");
  text = "";
  command = "";

  screen = gui->createScreen(window->getViewport(0), "dejavu");
  layer = screen->createLayer();
  view = layer->createMarkupText(9, 0, 0, "");

  update();
  setVisible(false);
}

Console::~Console(void)
{
}

void Console::setVisible(bool visible)
{
  layer->setVisible(visible);
}

bool Console::isVisible()
{
  return layer->isVisible();
}

void Console::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!isVisible()) return;
}

void Console::injectKeyDown(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;
  if(keyboard)
  {
    if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) isShift = true;
    else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) isControl = true;
  }
}

void Console::injectKeyUp(const OIS::KeyEvent &arg)
{
  if(!isVisible()) return;
  if(keyboard) 
  {
    if(arg.key == OIS::KC_SPACE) command += ' ';
    else if(arg.key == OIS::KC_GRAVE) return;
    else if(arg.key == OIS::KC_BACK) backspace();
    else if(arg.key == OIS::KC_RETURN) enter();
    else if(arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT) isShift = false;
    else if(arg.key == OIS::KC_LCONTROL || arg.key == OIS::KC_RCONTROL) isControl = false;
    else if(isShift) 
    {
      if(arg.key == OIS::KC_0) command += ')';
      else if(arg.key == OIS::KC_1) command += '!';
      else if(arg.key == OIS::KC_2) command += '@';
      else if(arg.key == OIS::KC_3) command += '#';
      else if(arg.key == OIS::KC_4) command += '$';
      else if(arg.key == OIS::KC_5) command += "%%";//used as escape sequence in gorilla markup
      else if(arg.key == OIS::KC_6) command += '^';
      else if(arg.key == OIS::KC_7) command += '&';
      else if(arg.key == OIS::KC_8) command += '*';
      else if(arg.key == OIS::KC_9) command += '(';

      else if(arg.key == OIS::KC_PERIOD) command += '>';
      else if(arg.key == OIS::KC_COMMA) command += '<';
      else if(arg.key == OIS::KC_SEMICOLON) command += ':';
      else if(arg.key == OIS::KC_APOSTROPHE) command += '"';
      else if(arg.key == OIS::KC_SLASH) command += '?';
      else if(arg.key == OIS::KC_MINUS) command += '_';
      else if(arg.key == OIS::KC_EQUALS) command += '+';
      else if(arg.key == OIS::KC_BACKSLASH) command += '|';

      else command += keyboard->getAsString(arg.key);
    }
    else 
    {
      std::string input = keyboard->getAsString(arg.key);
      input = std::tolower(input[0]);
      command += input;
    }
  }
  update();
}


void Console::enter()
{
  text += "\n> " + command;

  std::vector<std::string> elements;
  split(command, ' ', elements);
  
  if(elements.size() == 0) {}
  else if(elements.size() == 1)
  {
    if(elements[0] == "help") help();
    else if(elements[0] == "clear") clear();
    else if(elements[0] == "stats") stats();
    else if(elements[0] == "date") text += "\n%5" + world->getEnvironment()->getDateString() + "%r";
    else if(elements[0] == "moon") text += "\n%5" + world->getEnvironment()->getMoonPhase() + "%r";
    else if(elements[0] == "freeze") world->getEnvironment()->freezeCollisionDebug = true;
    else if(elements[0] == "unfreeze") world->getEnvironment()->freezeCollisionDebug = false;
    else if(elements[0] == "freecam") world->getEnvironment()->freeCameraDebug = !world->getEnvironment()->freeCameraDebug;
    else if(elements[0] == "hide") setVisible(false);
    else if(elements[0] == "numcells") display("number of cells", Ogre::StringConverter::toString(world->getNumberCells()));
    else if(elements[0] == "size") display("window size", Ogre::StringConverter::toString(window->getWidth()) + "x" + Ogre::StringConverter::toString(window->getHeight()));
    else if(elements[0] == "screenshot")
    {
      setVisible(false);
      window->writeContentsToTimestampedFile("screenshot", ".jpg");
      setVisible(true);
    }
    else if(elements[0] == "cells")
    {
      std::vector<Ogre::String> names;
      world->getCellNames(names);
      for(std::vector<Ogre::String>::iterator it = names.begin(); it < names.end(); ++it) display(*it);
    }
    else error("there is no command '" + command + "'");
  }
  else if(elements.size() == 2)
  {
    if(elements[0] == "go")//moves the player to the specified cell
    {
      Cell* target = world->getCell(elements[1]);
      if(target) world->movePlayer(world->getPlayer(), target);
      else error("no cell named '" + elements[1] + "'");
    }
    else error("there is no command '" + command + "'");
  }
  else if(elements.size() == 3)
  {
    if(elements[0] == "fullscreen" || elements[0] == "window") 
    {
      int width = std::atoi(elements[1].c_str());
      int height = std::atoi(elements[2].c_str());

      if(width == 0 || height == 0) error("bad screen dimensions");
      else window->setFullscreen((elements[0] == "fullscreen"), width, height);
    }
    else error("there is no command '" + command + "'");
  }
  else if(elements.size() == 6)
  {
    if(elements[0] == "add")
    {
      Cell* target = world->getCell(elements[2]);
      if(target)
      {
        int x = std::atoi(elements[3].c_str());
        int y = std::atoi(elements[4].c_str());
        int z = std::atoi(elements[5].c_str());
        Ogre::Vector3 position = Ogre::Vector3(x, y, z);
        if(elements[1] == "item") target->addItem(position);
        else if(elements[1] == "monster") target->addMonster(position);
        else error("no object type '" + elements[1] + "' exists");
      }
      else error("no cell named '" + elements[2] + "'");
    }
    else error("there is no command '" + command + "'");
  }
  else error("there is no command '" + command + "'");
  
  command = "";
}

void Console::help()
{
  display("help", "shows this list");
  display("stats", "shows a list of rendering statistics");
  display("clear", "clears the console");
  display("date", "displays the date");
  display("moon", "displays the moon phase");
  display("freeze", "freezes physics");
  display("unfreeze", "unfreezes physics");
  display("freecam", "frees the camera");
  display("hide", "hides the console");
  display("numcells", "displays the number of active cells");
  display("fullscreen [width] [height]", "sets the window to fullscreen mode");
  display("window [width] [height]", "sets the window to windowed mode");
  display("size", "displays the window dimensions");
  display("add [type] [cell name] [x] [y] [z]", "adds the given type of entity to the named cell at the given coordinates");
  display("screenshot", "takes a screenshot and outputs to timestamped file");
  display("go [cell name]", "moves the player to the specified cell");
  display("cells","lists loaded cells");
}

void Console::clear()
{
  text = "";
  command = "";
}

void Console::update()
{
  view->text(text + "\n%3> " + command + "_");
}

void Console::backspace()
{
  if(command.size() == 0) return;
  command = command.substr(0, command.size() - 1);
}

void Console::stats()
{
  display("average fps", Ogre::StringConverter::toString(int(window->getAverageFPS())));
  display("best fps", Ogre::StringConverter::toString(window->getBestFPS()));
  display("batches", Ogre::StringConverter::toString(window->getBatchCount()));
  display("colour depth", Ogre::StringConverter::toString(window->getColourDepth()));
  display("number of viewports", Ogre::StringConverter::toString(window->getNumViewports()));
  display("triangle count", Ogre::StringConverter::toString(window->getTriangleCount()));
}

void Console::display(Ogre::String comment)
{
  text += "\n" + comment;
}

void Console::display(Ogre::String highlight, Ogre::String comment)
{
  text += "\n%5" + highlight + "%r - " + comment;
}

void Console::display(bool option)
{
  if(option) text += "\nenabled";
  else text += "\ndisabled";
}

void Console::error(Ogre::String comment)
{
  text += "\n%4error%r - " + comment;
}

void Console::split(const std::string &s, char delim, std::vector<std::string> &elems) 
{
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) elems.push_back(item);
}

