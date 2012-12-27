
//Local
#include "World.h"
#include "Scene.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "ScriptManager.h"
#include "Console.h"
#include "KeyboardMap.h"

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
    defaultScene(0)
{
  scriptManager->setWorld(this);
}

//-------------------------------------------------------------------------------------
World::~World(void)
{
  for(std::map<int, Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if(it->second) delete (it->second);
    it->second = NULL;
  }

  if(player) delete player;
  player = NULL;

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

  Console::getInstance().setWorld(this);
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
  timeManager.update(elapsedSeconds);

  if(player)
  {
    Scene* scene = player->getScene();
    if(scene) scene->update(elapsedSeconds);
  }

  return true;
}

//-------------------------------------------------------------------------------------
void World::keyPressed(const OIS::KeyEvent &arg)
{
  player->keyPressed(arg);
}

//-------------------------------------------------------------------------------------
void World::keyReleased(const OIS::KeyEvent &arg)
{
  player->keyReleased(arg);
}

//-------------------------------------------------------------------------------------
void World::mouseMoved(const OIS::MouseEvent &arg)
{
  player->mouseMoved(arg);
}

//-------------------------------------------------------------------------------------
void World::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->mousePressed(arg, id);
}

//-------------------------------------------------------------------------------------
void World::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->mouseReleased(arg, id);
}

//-------------------------------------------------------------------------------------
void World::parseIni(std::string filename)
{
  boost::property_tree::ptree pt;
  try
  {
    Ogre::FileInfoListPtr fileListPtr = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("Essential", filename);
    if(fileListPtr->size() < 1) throw NHException("could not find the path to the specified initialisation file");

    boost::property_tree::ini_parser::read_ini(fileListPtr->front().archive->getName() + "/" + fileListPtr->front().filename, pt);
  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)//need to provide default values
  {
    throw NHException("could not parse the initialisation file");
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    throw NHException("the initialisation file is malformed");
  }

  //General
  enablePhysics = getIniBool("General.EnablePhysics", &pt);
  enableAI = getIniBool("General.EnableAI", &pt);
  enableAudio = getIniBool("General.EnableAudio", &pt);

  //Controls
  controlManager.moveForward = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Forward", &pt));
  controlManager.moveLeft = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Left", &pt));
  controlManager.moveBack = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Back", &pt));
  controlManager.moveRight = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Right", &pt));
  controlManager.jump = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Jump", &pt));
  controlManager.run = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Run", &pt));
  controlManager.crouch = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Crouch", &pt));
  controlManager.interact = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Interact", &pt));
  controlManager.quickslots.push_back(KeyboardMap::getInstance().getAsKey(getIniString("Controls.QuickSlot1", &pt)));
  controlManager.quickslots.push_back(KeyboardMap::getInstance().getAsKey(getIniString("Controls.QuickSlot2", &pt)));
  controlManager.console = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Console", &pt));
  controlManager.exit = KeyboardMap::getInstance().getAsKey(getIniString("Controls.Exit", &pt));
  controlManager.leftHand = OIS::MB_Left;
  controlManager.rightHand = OIS::MB_Right;
  controlManager.addItem = OIS::KC_1;
  controlManager.addMonster = OIS::KC_2;
    
  //Environment
  enableHDR = getIniBool("Environment.HDR", &pt);
  enableBloom = getIniBool("Environment.Bloom", &pt);
  enableSSAO = getIniBool("Environment.SSAO", &pt);
  enableMotionBlur = getIniBool("Environment.MotionBlur", &pt);
  enableShadows = getIniBool("Environment.Shadows", &pt);
  enableLights = getIniBool("Environment.Lights", &pt);
  enableParticles = getIniBool("Environment.Particles", &pt);
  enableDecals = getIniBool("Environment.Decals", &pt);
  enableSprites = getIniBool("Environment.Sprites", &pt);
  enableWater = getIniBool("Environment.Water", &pt);
  enableSky = getIniBool("Environment.Sky", &pt);

  //Data
  dataManager.addData(getIniString("Data.Scenes", &pt));
  dataManager.addData(getIniString("Data.Architecture", &pt));
  dataManager.addData(getIniString("Data.Monsters", &pt));
  dataManager.addData(getIniString("Data.Items", &pt));
  dataManager.addData(getIniString("Data.Sounds", &pt));

  //Debug
  freeCameraDebug = getIniBool("Debug.FreeCamera", &pt);
  wireframeDebug = getIniBool("Debug.Wireframe", &pt);
  freezeCollisionDebug = getIniBool("Debug.FreezeCollision", &pt);
  showCollisionDebug = getIniBool("Debug.ShowCollisionsDebug", &pt);
  showShadowDebug = getIniBool("Debug.ShowShadowDebug", &pt);
  defaultScene = getIniInt("Debug.DefaultScene", &pt);

  physXVisualDebuggerIP = getIniString("Debug.PhysXVisualDebuggerIP", &pt);
  physXVisualDebuggerPort = getIniInt("Debug.PhysXVisualDebuggerPort", &pt);
  physXVisualDebuggerTimeoutMilliseconds = getIniInt("Debug.PhysXVisualDebuggerTimeoutMilliseconds", &pt);
}

//-------------------------------------------------------------------------------------
std::string World::getIniString(std::string iniLabel, boost::property_tree::ptree* pt)
{
  try
  {
    return pt->get<std::string>(iniLabel);
  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)//need to provide default values
  {
    std::cerr << "failed to parse the ini file for the label " << iniLabel << std::endl;
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    std::cerr << "failed to parse the ini file for the label " << iniLabel << std::endl;
  }
  return "";
}

//-------------------------------------------------------------------------------------
int World::getIniInt(std::string iniLabel, boost::property_tree::ptree* pt)
{
  try
  {
    return pt->get<int>(iniLabel);
  }
  catch(boost::property_tree::ini_parser::ini_parser_error e)//need to provide default values
  {
    std::cerr << "failed to parse the ini file for the label " << iniLabel << std::endl;
  }
  catch(boost::property_tree::ptree_bad_path e)
  {
    std::cerr << "failed to parse the ini file for the label " << iniLabel << std::endl;
  }
  return 0;
}

//-------------------------------------------------------------------------------------
bool World::getIniBool(std::string iniLabel, boost::property_tree::ptree* pt)
{
  return (getIniString(iniLabel, pt) == "true");
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

//-------------------------------------------------------------------------------------
void World::setPhysicsEnabled(bool enabled)
{
  freezeCollisionDebug = !enabled;
}

//-------------------------------------------------------------------------------------
bool World::isPhysicsEnabled()
{
  return !freezeCollisionDebug;
}

//-------------------------------------------------------------------------------------
bool World::isShadowsEnabled()
{
  return enableShadows;
}
