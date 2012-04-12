#include "Console.h"

Console::Console(void)
  : visible(false)
{
}

Console::~Console(void)
{
}

void Console::show()
{
  visible = true;
}

void Console::hide()
{
  visible = false;
}

bool Console::isVisible()
{
  return visible;
}

void Console::injectEnter()
{
  if(command == "help") help();
  if(command == "clear") clear();
}

void Console::injectCharacter(Ogre::String character)
{
  command += character;
}

void Console::help()
{
  trace += "help\n";
}

void Console::clear()
{
  trace = "";
}