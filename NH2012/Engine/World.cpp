
//Local
#include "World.h"
#include "Scene.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"

//-------------------------------------------------------------------------------------
World::World(Ogre::Root* root)
  : root(root),
    scenes(),
    timeManager(),
    dataManager(),
    soundManager(),
    physicsManager(),
    controlManager(),
    fabricationManager(),
    player(NULL),
    sceneChangeListener(NULL), 
    defaultScene(0)
{
}

//-------------------------------------------------------------------------------------
World::~World(void)
{
  if(player) delete player;
  player = NULL;

  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if(*it) delete (*it);
    (*it) = NULL;
  }
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
  if(!scene) throw NHException("Default scene creation failed.");
  
  //creating player
  PlayerDesc playerDesc = PlayerDesc();
  playerDesc.gravity = scene->getGravity();
  player = new Player(playerDesc, this);
  if(!player) throw NHException("Initial player creation failed.");
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
Scene* World::getScene(Ogre::String name)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->getName() == name) return (*it);
  }
  return NULL;
}

//-------------------------------------------------------------------------------------
Scene* World::getScene(int id)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->getSceneID() == id) return (*it);
  }
  return NULL;
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
void World::getSceneNames(std::vector<Ogre::String> &names)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    names.push_back((*it)->getName());
  }
}

//-------------------------------------------------------------------------------------
Scene* World::loadScene(int id)
{
  if(hasScene(id)) return getScene(id);//check that the scene has not been loaded already
  Scene* scene = new Scene(dataManager.getScene(id), this);
  scenes.push_back(scene);
  return scene;
}

//-------------------------------------------------------------------------------------
bool World::hasScene(int id)
{
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it)
  {
    if((*it)->getSceneID() == id) return true;
  }
  return false;
}

//-------------------------------------------------------------------------------------
bool World::destroyScene(Ogre::String name)
{
  //is unsafe

  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->getName() == name) 
    {
      Scene* scene = *it;
      if(scene->isActive()) return false;//can't destroy active scene
      scenes.erase(it);
      delete scene;
      return true;
    }
  }
  return false;
}

//-------------------------------------------------------------------------------------
bool World::update(double elapsedSeconds)
{
  //if(waterManager) waterManager->update(elapsedSeconds);
  for(std::vector<Scene*>::iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    if((*it)->isActive()) (*it)->update(elapsedSeconds);//only send frame events to the active scenes
  }
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
    if(fileListPtr->size() < 1) throw NHException("Could not find the path to the specified initialisation file.");

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

    //Data File Paths
    dataManager.addData(pt.get<std::string>("Scenes.Data"));
    dataManager.addData(pt.get<std::string>("Architecture.Data"));
    dataManager.addData(pt.get<std::string>("Monsters.Data"));
    dataManager.addData(pt.get<std::string>("Items.Data"));
    dataManager.addData(pt.get<std::string>("Sounds.Data"));

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
  catch(NHException e)
  {
    std::cout << "Initialisation Error: " << e.what() << std::endl;
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


