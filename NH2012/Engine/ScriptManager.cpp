#include "ScriptManager.h"


ScriptManager::ScriptManager(void)
  : world(NULL)
{
}


ScriptManager::~ScriptManager(void)
{
}


void ScriptManager::setWorld(World* world)
{
  this->world = world;
}