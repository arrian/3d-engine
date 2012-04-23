#include "World.h"


World::World(Ogre::Root* root)
  : Environment(),
    root(root),
    cells(std::vector<Cell*>()),
    player(0),
    physicsFoundation(0),
    physicsWorld(0)
{
  //loading initialisation file
  //TODO extract absolute path and use boost to get relative path from current exe
  //TODO post-compile event copy ini file to executable directory
  parseIni("C:\\Dev\\Nethack2012\\NH2012\\Media\\nh2012.ini");
  
  //creating physics
  physx::PxAllocatorCallback* allocator = &allocatorCallback;
  physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, getErrorCallback());
  bool recordMemoryAllocations = true;
  physicsWorld = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxTolerancesScale(), recordMemoryAllocations);
  if(!physicsWorld) std::cout << "physics world could not be created" << std::endl;

  //creating initial cell
  loadCell(defaultStartLevel, FILE_CHAR);
  
  //creating player and placing in initial cell
  player = new Player(this);
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
}

void World::hookWindow(Ogre::RenderWindow* window)
{
  assert(player);
  player->hook(window);
}

Player* World::getPlayer()
{
  return player;
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

Ogre::Root* World::getRoot()
{
  return root;
}

physx::PxPhysics* World::getPhysics()
{
  return physicsWorld;
}

const physx::PxTolerancesScale& World::getTolerancesScale()
{
  return physicsWorld->getTolerancesScale();
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

bool World::loadCell(Ogre::String name, SceneType type)
{
  cells.push_back(new Cell(this, name, type));
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

physx::PxErrorCallback& World::getErrorCallback()
{
  static PhysicsErrorCallback defaultCallback;
  return defaultCallback;
}

