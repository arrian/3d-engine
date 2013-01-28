#pragma once

//Standard
#include <map>
#include <string>

//Controls
#include <OISKeyboard.h>
#include <OISMouse.h>

//Ogre
#include <OgreRoot.h>

//Local
#include "DataManager.h"
#include "TimeManager.h"
#include "SoundManager.h"
#include "ControlManager.h"
#include "PhysicsManager.h"
#include "ScriptManager.h"
#include "NetworkManager.h"

//Forward Declarations
class Player;
class Scene;

/************************************************************************/
/* Contains the entire game world.                                      */
/************************************************************************/
class World
{
public:
  World(Ogre::Root* root = NULL);
  ~World(void);

  bool update(double elapsedSeconds);

  //Initialisation
  void initialise(std::string iniFile);
  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player
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
  Ogre::Root* getRoot();
  Player* getPlayer();
  Scene* getScene(int id);
  int getNumberScenes();
  void getSceneNames(std::map<int, std::string> &names);

  //Setters
  void setRoot(Ogre::Root* root);
  void setPhysicsEnabled(bool enabled);
  void setBloomEnabled(bool enabled);
  void setSSAOEnabled(bool enabled);
  void setAIEnabled(bool enabled);
  void setAudioEnabled(bool enabled);
  /*
  void setHDREnabled(bool enabled);
  void setShadowsEnabled(bool enabled);
  void setLightingEnabled(bool enabled);
  void setEffectsEnabled(bool enabled);
  void setDecalsEnabled(bool enabled);
  void setWaterEnabled(bool enabled);
  void setSkyEnabled(bool enabled);
  void setFreeCameraEnabled(bool enabled);
  void setWireframeEnabled(bool enabled);
  */

  //Input
  void keyPressed(const OIS::KeyEvent &arg);
  void keyReleased(const OIS::KeyEvent &arg);
  void mouseMoved(const OIS::MouseEvent &arg);
  void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  //Assertions
  bool hasScene(int id);
  bool isPhysicsEnabled();
  bool isShadowsEnabled();
  bool isBloomEnabled();
  bool isSSAOEnabled();
  bool isAIEnabled();
  bool isAudioEnabled();
  /*
  bool isHDREnabled();
  bool isLightingEnabled();
  bool isEffectsEnabled();
  bool isDecalsEnabled();
  bool isWaterEnabled();
  bool isSkyEnabled();
  bool isFreeCameraEnabled();
  bool isWireframeEnabled();
  */

private:
  Ogre::Root* root;
  
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
  
  //Initialisation
  void parseInitialisation(std::string filename);

  //Flags
  bool enableShadows;
  bool enablePhysics;
  bool enableBloom;
  bool enableSSAO;
  bool enableAI;
  bool enableAudio;

};

