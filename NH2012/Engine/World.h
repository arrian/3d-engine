#pragma once

#include <vector>

#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreFrameListener.h>

#include <PxPhysX.h>
#include "PxPhysicsAPI.h"

//#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"

#include "SceneType.h"
#include "Cell.h"
#include "Environment.h"
#include "Player.h"
#include "DataManager.h"
#include "PhysicsErrorCallback.h"

class Player;
class Cell;

class World : public Environment
{
public:
  World(Ogre::Root* root);
  ~World(void);

  Player* getPlayer();
  Cell* getCell(Ogre::String name);
  Cell* getCell(unsigned int index);

  Ogre::Root* getRoot();
  physx::PxPhysics* getPhysics();
  const physx::PxTolerancesScale& getTolerancesScale();//messy tolerance bind

  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player

  /* Moves the player to the target cell.*/
  void movePlayer(Player* player, Cell* target);

  int getNumberCells();
  void getCellNames(std::vector<Ogre::String> &names);

  /* Loads a cell. Returns true if success.*/
  bool loadCell(Ogre::String name, SceneType type);

  /* Destroys a cell. Returns true if success.*/
  bool destroyCell(Ogre::String name);

  void setSceneManager(Ogre::SceneManager* sceneManager);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations

  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
private:
  Ogre::Root* root;
  
  physx::PxDefaultAllocator allocatorCallback;//for allocating memory to physics manager
  physx::PxFoundation* physicsFoundation;
  physx::PxPhysics* physicsWorld;

  Player* player;
  //std::vector<Player> players;
  //std::vector<Monster*> monsters;//should make monsters worlwide objects
  std::vector<Cell*> cells;

  physx::PxErrorCallback& World::getErrorCallback();
};

