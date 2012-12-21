#pragma once

//Standard
#include <map>
#include <ctime>
#include <iostream>
#include <string>

//Boost
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/asio/ip/address.hpp>

//Controls
#include <OISKeyboard.h>
#include <OISMouse.h>

//Ogre
#include <OgreFrameListener.h>
#include <OgreVector3.h>
#include <OgreString.h>

//Local
#include "DataManager.h"
#include "TimeManager.h"
#include "SoundManager.h"
#include "ScriptManager.h"
#include "ControlManager.h"
#include "PhysicsManager.h"
#include "FabricationManager.h"
#include "PhysicsErrorCallback.h"
#include "SceneChangeListener.h"
#include "NHException.h"

//Forward Declarations
class Player;
class Scene;


/************************************************************************/
/* Contains the entire game world. Multiple scenes can be loaded at once*/
/* (lazy loading between scenes will be implemented in the future)      */
/************************************************************************/
class World
{
public:
  World(Ogre::Root* root = NULL);
  ~World(void);

  bool update(double elapsedSeconds);

  //Initialisation
  void initialise(std::string iniFile);
  void parseIni(std::string filename);
  Scene* loadScene(int id);

  //Destruction
  bool destroyScene(int id);

  //Getters
  DataManager* getDataManager();
  TimeManager* getTimeManager();
  SoundManager* getSoundManager();
  ScriptManager* getScriptManager();
  PhysicsManager* getPhysicsManager();
  ControlManager* getControlManager();
  FabricationManager* getFabricationManager();
  Player* getPlayer();
  Ogre::Root* getRoot();
  int getNumberScenes();
  Scene* getScene(int id);
  SceneChangeListener* getSceneChangeListener();
  void getSceneNames(std::map<int, std::string> &names);

  //Setters
  void setRoot(Ogre::Root* root);
  void setSceneChangeListener(SceneChangeListener* listener);
  void setSceneManager(Ogre::SceneManager* sceneManager);
  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player
  
  //Injection
  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  //Assertions
  bool hasScene(int id);

  //Flags... eventually remove
  bool enablePhysics;
  bool enableAI;
  bool enableAudio;
  bool enableHDR;
  bool enableBloom;
  bool enableSSAO;
  bool enableMotionBlur;
  bool enableShadows;
  bool enableLights;
  bool enableParticles;
  bool enableDecals;
  bool enableSprites;
  bool enableWater;
  bool enableSky;
  bool freeCameraDebug;
  bool wireframeDebug;
  bool freezeCollisionDebug;
  bool showCollisionDebug;
  bool showShadowDebug;

private:
  int defaultScene;
  std::string physXVisualDebuggerIP;
  int physXVisualDebuggerPort;
  int physXVisualDebuggerTimeoutMilliseconds;

  Ogre::Root* root;
  SceneChangeListener* sceneChangeListener;

  DataManager dataManager;
  TimeManager timeManager;
  SoundManager soundManager;
  ScriptManager scriptManager;
  PhysicsManager physicsManager;
  ControlManager controlManager;
  FabricationManager fabricationManager;
  
  Player* player;
  std::map<int, Scene*> scenes;
};

