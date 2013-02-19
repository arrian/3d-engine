#pragma once

//Standard
#include <map>
#include <string>

//Local
#include "DataManager.h"
#include "TimeManager.h"
#include "SoundManager.h"
#include "ScriptManager.h"
#include "ControlManager.h"
#include "PhysicsManager.h"
#include "NetworkManager.h"
#include "GraphicsManager.h"

//Forward Declarations
class Player;
class Scene;

/************************************************************************/
/* Contains the entire game world.                                      */
/************************************************************************/
class World
{
public:
  World();
  ~World(void);

  bool update(double elapsedSeconds);

  //Initialisation
  void initialise(std::string iniFile);
  Scene* loadScene(int id);

  //Destruction
  void destroyScene(int id);

  //Getters
  DataManager* getDataManager();
  TimeManager* getTimeManager();
  SoundManager* getSoundManager();
  ScriptManager* getScriptManager();
  PhysicsManager* getPhysicsManager();
  ControlManager* getControlManager();
  NetworkManager* getNetworkManager();
  GraphicsManager* getGraphicsManager();

  //Ogre::Root* getRoot();
  Player* getPlayer();
  Scene* getScene(int id);
  int getNumberScenes();
  void getSceneNames(std::map<int, std::string> &names);

  //Input
  void keyPressed(const OIS::KeyEvent &arg);
  void keyReleased(const OIS::KeyEvent &arg);
  void mouseMoved(const OIS::MouseEvent &arg);
  void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  //Assertions
  bool hasScene(int id);

private:
  //Local Player
  Player* player;

  //Scenes
  int defaultScene;
  std::map<int, Scene*> scenes;
  
  //Managers
  DataManager dataManager;
  TimeManager timeManager;
  SoundManager soundManager;
  ScriptManager scriptManager;
  PhysicsManager physicsManager;
  ControlManager controlManager;
  NetworkManager networkManager;
  GraphicsManager graphicsManager;
  
  //Initialisation
  void parseInitialisation(std::string filename);
};

