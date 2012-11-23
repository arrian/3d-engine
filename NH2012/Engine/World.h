#pragma once

//Standard
#include <vector>
#include <ctime>
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>

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

//PhysX
#include "PxPhysicsAPI.h"
//#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include "cooking/PxCooking.h"

//Local
#include "Controls.h"
#include "DataManager.h"
#include "SoundManager.h"
#include "FabricationManager.h"
#include "PhysicsErrorCallback.h"
#include "SceneChangeListener.h"
#include "NHException.h"

//Forward Declarations
class Player;
class Monster;
class Item;
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

  //Getters
  DataManager* getDataManager();
  FabricationManager* getFabricationManager();
  Player* getPlayer();
  Scene* getScene(Ogre::String name);
  Scene* getScene(int id);
  Ogre::Root* getRoot();
  physx::PxPhysics* getPhysics();
  int getNumberScenes();
  void getSceneNames(std::vector<Ogre::String> &names);
  physx::PxMaterial* getDefaultPhysicsMaterial();
  SceneChangeListener* getSceneChangeListener();
  const physx::PxTolerancesScale& getTolerancesScale();//messy tolerance bind

  //Creation
  //Item* createItem(int id);
  //Monster* createMonster(int id);

  //Removal
  //void releaseItem(Item* item);
  //void releaseMonster(Monster* monster);
  bool destroyScene(Ogre::String name);

  //Setters
  void setRoot(Ogre::Root* root);
  void setSceneChangeListener(SceneChangeListener* listener);
  void setSceneManager(Ogre::SceneManager* sceneManager);
  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player
  //void movePlayer(Player* player, Scene* target);/* Moves the player to the target scene.*/
  
  //Injection
  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  //Assertions
  bool hasScene(int id);
  bool isDebug();

  //Flags... extract to settings class
  bool debug;
  bool verbose;
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
  float gravity;
  int defaultScene;
  std::vector<std::string> dataFiles;//monsters,items,architecture,sounds,scenes etc.
  Controls controls;//Control mappings

private:

  Ogre::Root* root;
  SceneChangeListener* sceneChangeListener;
  DataManager dataManager;
  SoundManager soundManager;
  FabricationManager fabricationManager;
  
  //World Contents
  Player* player;//std::vector<Player> players;
  //std::vector<Monster*> monsters;
  //std::vector<Item*> items;
  std::vector<Scene*> scenes;

  //PhysX
  PhysicsErrorCallback errorCallback;
  physx::PxDefaultAllocator allocatorCallback;//for allocating memory to physics manager
  physx::PxFoundation* physicsFoundation;
  physx::PxCooking* physicsCooking;
  physx::PxPhysics* physicsWorld;
  physx::PxMaterial* physicsMaterial;//default material

  //Defaults
  physx::PxReal defaultStaticFriction;
  physx::PxReal defaultDynamicFriction;
  physx::PxReal defaultRestitution;

  std::string physXVisualDebuggerIP;
  int physXVisualDebuggerPort;
  int physXVisualDebuggerTimeoutMilliseconds;

};

