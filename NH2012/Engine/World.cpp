#include "World.h"


World::World(Ogre::Root* root, Ogre::RenderWindow* window)
  : root(root),
    environment(new Environment()),
    cells(std::vector<Cell*>()),
    player(0)
{
  //TODO extract absolute path and use boost to get relative path from current exe
  //TODO post-compile event copy ini file to executable directory
  environment->parseIni("C:\\Dev\\Nethack2012\\NH2012\\Media\\nh2012.ini");

  //loadCell("Entrance", CellType::DUNGEON);
  //loadCell("Foyer", CellType::PREDEFINED);
  loadCell("C:\\Dev\\Nethack2012\\NH2012\\Media\\default.level", CellType::FILE);
  
  player = new Player(environment, window);
  getCell("test")->addPlayer(player);
}

World::~World(void)
{
  if(player) delete player;
  player = 0;

  for(std::vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) 
  {
    if(*it) delete (*it);
    (*it) = 0;
  }

  if(environment) delete environment;
  environment = 0;
}

Player* World::getPlayer()
{
  return player;
}

Environment* World::getEnvironment()
{
  return environment;
}

Cell* World::getCell(Ogre::String name)
{
  for(std::vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) 
  {
    if((*it)->getName() == name) return (*it);
  }
  return 0;
}

Cell* World::getCell(unsigned int index)
{
  if(index < cells.size()) return cells[index];
  return 0;
}

int World::getNumberCells()
{
  return cells.size();
}

void World::movePlayer(Player* player, Cell* target)
{
  for(std::vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) 
  {
    (*it)->removePlayer(player);
  }
  target->addPlayer(player);
}

void World::getCellNames(std::vector<Ogre::String> &names)
{
  for(std::vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) 
  {
    names.push_back((*it)->getName());
  }
}

bool World::loadCell(Ogre::String name, CellType::Type type)
{
  cells.push_back(new Cell(root, environment, name, type));
  return true;
}

bool World::destroyCell(Ogre::String name)
{
  //is unsafe

  for(std::vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) 
  {
    if((*it)->getName() == name) 
    {
      Cell* cell = *it;
      if(cell->isActive()) return false;//can't destroy active cell
      cells.erase(it);
      delete cell;
      return true;
    }
  }
  return false;
}

bool World::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  for(std::vector<Cell*>::iterator it = cells.begin(); it != cells.end(); ++it) 
  {
    if((*it)->isActive()) (*it)->frameRenderingQueued(evt);//only send frame events to the active cells
  }
  return true;
}

void World::injectKeyDown(const OIS::KeyEvent &arg)
{
  player->injectKeyDown(arg);
}

void World::injectKeyUp(const OIS::KeyEvent &arg)
{
  player->injectKeyUp(arg);
}

void World::injectMouseMove(const OIS::MouseEvent &arg)
{
  player->injectMouseMove(arg);
}

void World::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseDown(arg, id);
}

void World::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseUp(arg, id);
}

