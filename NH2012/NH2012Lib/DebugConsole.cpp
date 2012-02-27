#include "DebugConsole.h"

DebugConsole::DebugConsole(void)
  : visible(false)
{
}

DebugConsole::~DebugConsole(void)
{
}

void DebugConsole::show()
{
  visible = true;
}

void DebugConsole::hide()
{
  visible = false;
}

bool DebugConsole::isVisible()
{
  return visible;
}

void DebugConsole::injectEnter()
{
  if(command == "help") help();
  if(command == "clear") clear();
}

void DebugConsole::injectCharacter(Ogre::String character)
{
  command += character;
}

void DebugConsole::help()
{
  trace += "help\n";
}

void DebugConsole::clear()
{
  trace = "";
}