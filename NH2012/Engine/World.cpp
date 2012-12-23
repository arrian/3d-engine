
//Local
#include "World.h"
#include "Scene.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "ScriptManager.h"

//-------------------------------------------------------------------------------------
World::World(Ogre::Root* root)
  : root(root),
    scenes(),
    timeManager(),
    dataManager(),
    soundManager(),
    scriptManager(new ScriptManager()),
    physicsManager(),
    controlManager(),
    fabricationManager(),
    player(NULL),
    sceneChangeListener(NULL), 
    defaultScene(0)
{
  scriptManager->setWorld(this);
}

//-------------------------------------------------------------------------------------
World::~World(void)
{
  if(player) delete player;
  player = NULL;

  for(std::map<int, Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if(it->second) delete (it->second);
    it->second = NULL;
  }

  if(scriptManager) delete scriptManager;
}

//-------------------------------------------------------------------------------------
void World::initialise(std::string iniFile)
{
  parseIni(iniFile);

  fabricationManager.setPhysics(physicsManager.getPhysics());
  fabricationManager.setCooking(physicsManager.getCooking());
  fabricationManager.setDefaultPhysicsMaterial(physicsManager.getDefaultMaterial());//move all material stuff to the fabrication manager

#ifdef _DEBUG
  physicsManager.initialiseVisualDebugger(physXVisualDebuggerIP, physXVisualDebuggerPort, physXVisualDebuggerTimeoutMilliseconds);
#endif

  //creating default scene
  Scene* scene = loadScene(defaultScene);
  if(!scene) throw NHException("default scene creation failed");
  
  //creating player
  PlayerDesc playerDesc = PlayerDesc();
  playerDesc.gravity = scene->getGravity();
  player = new Player(playerDesc, this);
  if(!player) throw NHException("initial player creation failed");
  scene->addPlayer(player);
}

//-------------------------------------------------------------------------------------
void World::hookWindow(Ogre::RenderWindow* window)
{
  assert(player);
  player->hookWindow(window);
}

//-------------------------------------------------------------------------------------
Player* World::getPlayer()
{
  return player;
}

//-------------------------------------------------------------------------------------
Scene* World::getScene(int id)
{
  if(!hasScene(id)) return NULL;
  return scenes.find(id)->second;
}

//-------------------------------------------------------------------------------------
int World::getNumberScenes()
{
  return scenes.size();
}

//-------------------------------------------------------------------------------------
Ogre::Root* World::getRoot()
{
  return root;
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
  return scriptManager;
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
bool World::hasScene(int id)
{
  return scenes.count(id) > 0;
}

//-------------------------------------------------------------------------------------
bool World::destroyScene(int id)
{
  if(!hasScene(id)) return false;
  Scene* scene = scenes.find(id)->second;
  if(scene->isActive()) return false;
  delete scene;
  scenes.erase(scenes.find(id));
  return true;
}

//-------------------------------------------------------------------------------------
bool World::update(double elapsedSeconds)
{
  if(!scriptManager->update(elapsedSeconds)) return false;

  if(player && player->getScene()) player->getScene()->update(elapsedSeconds);//only update the active scene

  timeManager.tick();

  return true;
}

//-------------------------------------------------------------------------------------
void World::injectKeyDown(const OIS::KeyEvent &arg)
{
  player->injectKeyDown(arg);
}

//-------------------------------------------------------------------------------------
void World::injectKeyUp(const OIS::KeyEvent &arg)
{
  player->injectKeyUp(arg);
}

//-------------------------------------------------------------------------------------
void World::injectMouseMove(const OIS::MouseEvent &arg)
{
  player->injectMouseMove(arg);
}

//-------------------------------------------------------------------------------------
void World::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseDown(arg, id);
}

//-------------------------------------------------------------------------------------
void World::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseUp(arg, id);
}

//-------------------------------------------------------------------------------------
void World::setSceneChangeListener(SceneChangeListener* listener)
{
  sceneChangeListener = listener;
}

//-------------------------------------------------------------------------------------
SceneChangeListener* World::getSceneChangeListener()
{
  return sceneChangeListener;
}

//-------------------------------------------------------------------------------------
void World::parseIni(std::string filename)
{
  try
  {
    Ogre::FileInfoListPtr fileListPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("Essential", filename);
    if(fileListPtr->size() < 1) throw NHException("could not find the path to the specified initialisation file");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(fileListPtr->front().archive->getName() + "/" + fileListPtr->front().filename, pt);

    std::string TRUE_STRING = "true";

    //General
    enablePhysics = (pt.get<std::string>("General.EnablePhysics") == TRUE_STRING);
    enableAI = (pt.get<std::string>("General.EnableAI") == TRUE_STRING);
    enableAudio = (pt.get<std::string>("General.EnableAudio") == TRUE_STRING);

    //Controls
    controlManager.moveForward = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Forward"));
    controlManager.moveLeft = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Left"));
    controlManager.moveBack = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Back"));
    controlManager.moveRight = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Right"));
    controlManager.jump = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Jump"));
    controlManager.run = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Run"));
    controlManager.crouch = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Crouch"));
    controlManager.interact = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Interact"));
    controlManager.quickslots.push_back(controlManager.stringToKeyCode(pt.get<std::string>("Controls.QuickSlot1")));
    controlManager.quickslots.push_back(controlManager.stringToKeyCode(pt.get<std::string>("Controls.QuickSlot2")));
    controlManager.console = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Console"));
    controlManager.exit = controlManager.stringToKeyCode(pt.get<std::string>("Controls.Exit"));
    controlManager.leftHand = OIS::MB_Left;
    controlManager.rightHand = OIS::MB_Right;
    controlManager.addItem = OIS::KC_1;
    controlManager.addMonster = OIS::KC_2;
    
    //Environment
    enableHDR = (pt.get<std::string>("Environment.HDR") == TRUE_STRING);
    enableBloom = (pt.get<std::string>("Environment.Bloom") == TRUE_STRING);
    enableSSAO = (pt.get<std::string>("Environment.SSAO") == TRUE_STRING);
    enableMotionBlur = (pt.get<std::string>("Environment.MotionBlur") == TRUE_STRING);
    enableShadows = (pt.get<std::string>("Environment.Shadows") == TRUE_STRING);
    enableLights = (pt.get<std::string>("Environment.Lights") == TRUE_STRING);
    enableParticles = (pt.get<std::string>("Environment.Particles") == TRUE_STRING);
    enableDecals = (pt.get<std::string>("Environment.Decals") == TRUE_STRING);
    enableSprites = (pt.get<std::string>("Environment.Sprites") == TRUE_STRING);
    enableWater = (pt.get<std::string>("Environment.Water") == TRUE_STRING);
    enableSky = (pt.get<std::string>("Environment.Sky") == TRUE_STRING);

    //Data
    dataManager.addData(pt.get<std::string>("Data.Scenes"));
    dataManager.addData(pt.get<std::string>("Data.Architecture"));
    dataManager.addData(pt.get<std::string>("Data.Monsters"));
    dataManager.addData(pt.get<std::string>("Data.Items"));
    dataManager.addData(pt.get<std::string>("Data.Sounds"));

    //Debug
    freeCameraDebug = (pt.get<std::string>("Debug.FreeCamera") == TRUE_STRING);
    wireframeDebug = (pt.get<std::string>("Debug.Wireframe") == TRUE_STRING);
    freezeCollisionDebug = (pt.get<std::string>("Debug.FreezeCollision") == TRUE_STRING);
    showCollisionDebug = (pt.get<std::string>("Debug.ShowCollisionsDebug") == TRUE_STRING);
    showShadowDebug = (pt.get<std::string>("Debug.ShowShadowDebug") == TRUE_STRING);
    defaultScene = pt.get<int>("Debug.DefaultScene");

    physXVisualDebuggerIP = pt.get<std::string>("Debug.PhysXVisualDebuggerIP");
    physXVisualDebuggerPort = pt.get<int>("Debug.PhysXVisualDebuggerPort");
    physXVisualDebuggerTimeoutMilliseconds = pt.get<int>("Debug.PhysXVisualDebuggerTimeoutMilliseconds");
  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)//need to provide default values
  {
    std::cout << e.what() << std::endl;
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    std::cout << e.what() << std::endl;
  }
}

//-------------------------------------------------------------------------------------
DataManager* World::getDataManager()
{
  return &dataManager;
}

//-------------------------------------------------------------------------------------
void World::setRoot(Ogre::Root* root)
{
  this->root = root;
}

//-------------------------------------------------------------------------------------
FabricationManager* World::getFabricationManager()
{
  return &fabricationManager;
}

//-------------------------------------------------------------------------------------
TimeManager* World::getTimeManager()
{
  return &timeManager;
}


