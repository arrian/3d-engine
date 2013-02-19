
#include "World.h"

//Boost
#include <boost/lexical_cast.hpp>

//Local
#include "Scene.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "ScriptManager.h"
#include "ControlMap.h"
#include "NHException.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "InitialisationParser.h"
#include "ScenePhysicsManager.h"

//-------------------------------------------------------------------------------------
World::World()
  : scenes(),
    timeManager(),
    dataManager(),
    soundManager(),
    scriptManager(),
    physicsManager(),
    controlManager(),
    networkManager(),
    player(NULL),
    defaultScene(0)
{
  scriptManager.setWorld(this);
}

//-------------------------------------------------------------------------------------
World::~World(void)
{
  for(std::map<int, Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    delete (it->second);
    it->second = NULL;
  }

  delete player;
  player = NULL;
}

//-------------------------------------------------------------------------------------
void World::initialise(std::string iniFile)
{
  parseInitialisation(iniFile);

  //Create default scene
  Scene* scene = loadScene(defaultScene);
  if(!scene) throw NHException("default scene creation failed");
  
  //Create default player
  PlayerDesc playerDesc = PlayerDesc();
  playerDesc.gravity = scene->getScenePhysicsManager()->getGravity();
  player = new Player(playerDesc, this);
  scene->addPlayer(player);
  graphicsManager.setCamera(player->getCamera());
}

//-------------------------------------------------------------------------------------
bool World::update(double elapsedSeconds)
{
  if(!scriptManager.update(elapsedSeconds)) return false;
  timeManager.update(elapsedSeconds);

  if(player)
  {
    Scene* scene = player->getScene();
    if(scene) scene->update(elapsedSeconds);
  }

  return true;
}

//-------------------------------------------------------------------------------------
Player* World::getPlayer()
{
  return player;
}

//-------------------------------------------------------------------------------------
PhysicsManager* World::getPhysicsManager()
{
  return &physicsManager;
}

//-------------------------------------------------------------------------------------
ControlManager* World::getControlManager()
{
  return &controlManager;
}

//-------------------------------------------------------------------------------------
ScriptManager* World::getScriptManager()
{
  return &scriptManager;
}

//-------------------------------------------------------------------------------------
NetworkManager* World::getNetworkManager()
{
  return &networkManager;
}

//-------------------------------------------------------------------------------------
DataManager* World::getDataManager()
{
  return &dataManager;
}

//-------------------------------------------------------------------------------------
TimeManager* World::getTimeManager()
{
  return &timeManager;
}

//-------------------------------------------------------------------------------------
GraphicsManager* World::getGraphicsManager()
{
  return &graphicsManager;
}

//-------------------------------------------------------------------------------------
Scene* World::getScene(int id)
{
  if(!hasScene(id)) return NULL;
  return scenes.find(id)->second;
}

//-------------------------------------------------------------------------------------
bool World::hasScene(int id)
{
  return scenes.count(id) > 0;
}

//-------------------------------------------------------------------------------------
int World::getNumberScenes()
{
  return scenes.size();
}

//-------------------------------------------------------------------------------------
void World::getSceneNames(std::map<int, std::string> &names)
{
  for(std::map<int, Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    names.insert(std::pair<int, std::string>(it->first,it->second->getName()));
  }
}

//-------------------------------------------------------------------------------------
Scene* World::loadScene(int id)
{
  if(hasScene(id)) return getScene(id);//check that the scene has not been loaded already
  Scene* scene = new Scene(dataManager.getScene(id), this);
  scenes.insert(std::pair<int, Scene*>(scene->getSceneID(), scene));
  return scene;
}

//-------------------------------------------------------------------------------------
void World::destroyScene(int id)
{
  if(!hasScene(id)) throw NHException("there is no scene loaded with that id");
  Scene* scene = scenes.find(id)->second;
  if(scene->hasPlayer()) throw NHException("unable to destroy a scene while it contains a player");
  delete scene;
  scenes.erase(scenes.find(id));
}

//-------------------------------------------------------------------------------------
void World::keyPressed(const OIS::KeyEvent &arg)
{
  assert(player);
  player->keyPressed(arg);
}

//-------------------------------------------------------------------------------------
void World::keyReleased(const OIS::KeyEvent &arg)
{
  assert(player);
  player->keyReleased(arg);
}

//-------------------------------------------------------------------------------------
void World::mouseMoved(const OIS::MouseEvent &arg)
{
  assert(player);
  player->mouseMoved(arg);
}

//-------------------------------------------------------------------------------------
void World::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  assert(player);
  player->mousePressed(arg, id);
}

//-------------------------------------------------------------------------------------
void World::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  assert(player);
  player->mouseReleased(arg, id);
}

//-------------------------------------------------------------------------------------
void World::parseInitialisation(std::string filename)
{
  InitialisationParser ini(filename);

  //General
  physicsManager.setEnabled(ini.get<bool>("General.EnablePhysics"));
  soundManager.setEnabled(ini.get<bool>("General.EnableAudio"));
  //enableAI = ini.get<bool>("General.EnableAI");

  //Controls
  controlManager.moveForward = Button(ini.get<std::string>("Controls.Forward"));
  controlManager.moveLeft = Button(ini.get<std::string>("Controls.Left"));
  controlManager.moveBack = Button(ini.get<std::string>("Controls.Back"));
  controlManager.moveRight = Button(ini.get<std::string>("Controls.Right"));
  controlManager.jump = Button(ini.get<std::string>("Controls.Jump"));
  controlManager.run = Button(ini.get<std::string>("Controls.Run"));
  controlManager.crouch = Button(ini.get<std::string>("Controls.Crouch"));
  controlManager.interact = Button(ini.get<std::string>("Controls.Interact"));
  for(int i = 1; i < 10; i++) controlManager.quickslots.push_back(Button(ini.get<std::string>("Controls.QuickSlot" + boost::lexical_cast<std::string>(i))));
  controlManager.console = Button(ini.get<std::string>("Controls.Console"));
  controlManager.exit = Button(ini.get<std::string>("Controls.Exit"));
  controlManager.leftHand = Button(ini.get<std::string>("Controls.LeftHand"));//OIS::MB_Left;
  controlManager.rightHand = Button(ini.get<std::string>("Controls.RightHand"));//OIS::MB_Right;

  //Temp Controls
  controlManager.addItem = OIS::KC_1;
  controlManager.addMonster = OIS::KC_2;
  
  //Environment
  //graphicsManager.setShadowsEnabled(ini.get<bool>("Environment.Shadows"));
  //graphicsManager.setBloomEnabled(ini.get<bool>("Environment.Bloom"));
  //graphicsManager.setSSAOEnabled(ini.get<bool>("Environment.SSAO"));
  
  /*
  enableHDR = ini.get<bool>("Environment.HDR");
  enableMotionBlur = ini.get<bool>("Environment.MotionBlur");
  enableLights = ini.get<bool>("Environment.Lights");
  enableParticles = ini.get<bool>("Environment.Particles");
  enableDecals = ini.get<bool>("Environment.Decals");
  enableSprites = ini.get<bool>("Environment.Sprites");
  enableWater = ini.get<bool>("Environment.Water");
  enableSky = ini.get<bool>("Environment.Sky");
  */

  //Data
  dataManager.addData(ini.get<std::string>("Data.Scenes"));
  dataManager.addData(ini.get<std::string>("Data.Architecture"));
  dataManager.addData(ini.get<std::string>("Data.Monsters"));
  dataManager.addData(ini.get<std::string>("Data.Items"));
  dataManager.addData(ini.get<std::string>("Data.Sounds"));

  //Debug
  /*
  freezeCollisionDebug = ini.get<bool>("Debug.FreezeCollision");
  freeCameraDebug = ini.get<bool>("Debug.FreeCamera");
  wireframeDebug = ini.get<bool>("Debug.Wireframe");
  showCollisionDebug = ini.get<bool>("Debug.ShowCollisionsDebug");
  showShadowDebug = ini.get<bool>("Debug.ShowShadowDebug");
  */
  defaultScene = ini.get<int>("Debug.DefaultScene");

#ifdef _DEBUG
  std::string physXVisualDebuggerIP = ini.get<std::string>("Debug.PhysXVisualDebuggerIP");
  int physXVisualDebuggerPort = ini.get<int>("Debug.PhysXVisualDebuggerPort");
  int physXVisualDebuggerTimeoutMilliseconds = ini.get<int>("Debug.PhysXVisualDebuggerTimeoutMilliseconds");

  physicsManager.initialiseVisualDebugger(physXVisualDebuggerIP, physXVisualDebuggerPort, physXVisualDebuggerTimeoutMilliseconds);
#endif
}



