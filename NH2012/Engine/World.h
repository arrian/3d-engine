#pragma once

#include <vector>

#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreFrameListener.h>

#include "Cell.h"
#include "Environment.h"
#include "Player.h"

class World
{
public:
  World(Ogre::Root* root, Ogre::RenderWindow* window);
  ~World(void);

  Environment* getEnvironment();
  Player* getPlayer();
  Cell* getCell(Ogre::String name);
  Cell* getCell(unsigned int index);

  /**
   * Moves the player to the target cell.
   */
  void movePlayer(Player* player, Cell* target);

  int getNumberCells();
  void getCellNames(std::vector<Ogre::String> &names);

  void loadCell(Ogre::String name, CellType::Type type);
  void destroyCell(Cell* cell);
  void saveCell(Cell* cell);

  void setSceneManager(Ogre::SceneManager* sceneManager);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations

  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
private:
  Ogre::Root* root;
  Environment* environment;

  int timestamp;

  Player* player;
  //std::vector<Player> players;
  //std::vector<Monster*> monsters;//should make monsters worlwide objects
  std::vector<Cell*> cells;
};

