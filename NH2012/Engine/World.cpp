
//Local
#include "World.h"
#include "Scene.h"
#include "Player.h"
#include "Item.h"
#include "Monster.h"
#include "NHException.h"

//PhysX
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxVisualDebuggerExt.h"
#include "PxTkStream.h"

//-------------------------------------------------------------------------------------
World::World(Ogre::Root* root)
  : root(root),
    items(),
    scenes(),
    monsters(),
    dataFiles(),
    dataManager(),
    soundManager(),
    errorCallback(),
    allocatorCallback(),
    fabricationManager(),
    player(NULL),
    physicsWorld(NULL),
    physicsMaterial(NULL),
    physicsFoundation(NULL),
    sceneChangeListener(NULL),
    defaultRestitution(0.2f),
    defaultStaticFriction(0.3f),
    defaultDynamicFriction(0.4f),
    debug(false)
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

  physicsWorld->release();
  physicsWorld = NULL;

  physicsFoundation->release();
  physicsFoundation = NULL;
}

//-------------------------------------------------------------------------------------
void World::initialise(std::string iniFile)
{
  //loading initialisation file
  parseIni(iniFile);

  //Adding datafiles collected from ini file to dataManager
  for(std::vector<std::string>::iterator it = dataFiles.begin(); it < dataFiles.end(); ++it) dataManager.addData(*it);

  //creating physics
  physx::PxAllocatorCallback* allocator = &allocatorCallback;
  physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, errorCallback);//getErrorCallback());
  physicsFoundation->setErrorLevel(physx::PxErrorCode::eDEBUG_INFO);
  if(!physicsFoundation) throw NHException("Physics foundation could not be created.");

  bool recordMemoryAllocations = true;
  physicsWorld = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxTolerancesScale(), recordMemoryAllocations);
  if(!physicsWorld) throw NHException("Physics world could not be created.");

  PxInitExtensions(*physicsWorld);
  fabricationManager.setPhysics(physicsWorld);

  physicsCooking = PxCreateCooking(PX_PHYSICS_VERSION, physicsWorld->getFoundation(), physx::PxCookingParams());
  if(!physicsCooking) throw NHException("Physics cooker could not be created.");
  //should release cooking during gameplay
  fabricationManager.setCooking(physicsCooking);

  physicsMaterial = physicsWorld->createMaterial(defaultStaticFriction, defaultDynamicFriction, defaultRestitution);
  if(!physicsMaterial) throw NHException("Default physics material could not be created.");
  fabricationManager.setDefaultPhysicsMaterial(physicsMaterial);//move all material stuff to the fabrication manager

  //creating default scene
  Scene* scene = loadScene(defaultScene);
  if(!scene) throw NHException("Default scene creation failed.");
  
  //creating player
  player = new Player(this);
  if(!player) throw NHException("Initial player creation failed.");
  scene->addPlayer(player);

#ifdef _DEBUG
  //Creating connection to the PhysX Visual Debugger
  physx::PxExtensionVisualDebugger::connect(physicsWorld->getPvdConnectionManager(), physXVisualDebuggerIP.c_str(), physXVisualDebuggerPort, physXVisualDebuggerTimeoutMilliseconds);
#endif
}

//-------------------------------------------------------------------------------------
void World::hookWindow(Ogre::RenderWindow* window)
{
  assert(player);
  player->hook(window);
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
/*WARNING: May be a null pointer.*/
Ogre::Root* World::getRoot()
{
  return root;
}

//-------------------------------------------------------------------------------------
physx::PxPhysics* World::getPhysics()
{
  return physicsWorld;
}

//-------------------------------------------------------------------------------------
const physx::PxTolerancesScale& World::getTolerancesScale()
{
  return physicsWorld->getTolerancesScale();
}

//-------------------------------------------------------------------------------------
/*void World::movePlayer(Player* player, Scene* target)
{
  if(!player) return;
  Scene* old = player->getScene();
  if(old) old->removePlayer(player);
  if(target) target->addPlayer(player);
}*/

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
  Scene* scene = new Scene(this, id);
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
physx::PxMaterial* World::getDefaultPhysicsMaterial()
{
  return physicsMaterial;
}

//-------------------------------------------------------------------------------------
Item* World::createItem(int id)
{
  Item* item = new Item(dataManager.getItem(id));
  items.push_back(item);
  return item;
}

//-------------------------------------------------------------------------------------
Monster* World::createMonster(int id)
{
  Monster* monster = new Monster(id);
  monsters.push_back(monster);
  return monster;
}

//-------------------------------------------------------------------------------------
void World::releaseItem(Item* item)
{
  //!!! fix to delete from items list
  if(item) delete item;
}

//-------------------------------------------------------------------------------------
void World::releaseMonster(Monster* monster)
{
  //!!! fix to delete from monsters list
  if(monster) delete monster;
}

//-------------------------------------------------------------------------------------
void World::parseIni(std::string filename)
{
  std::cout << "Getting initialisation data..." << std::endl;

  try
  {
    std::ifstream s(filename);

    if(!s) throw NHException("Error opening initialisation file.");

    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(s, pt);

    std::string TRUE_STRING = "true";

    //General
    enablePhysics = (pt.get<std::string>("General.EnablePhysics") == TRUE_STRING);
    enableAI = (pt.get<std::string>("General.EnableAI") == TRUE_STRING);
    enableAudio = (pt.get<std::string>("General.EnableAudio") == TRUE_STRING);

    gravity = pt.get<float>("General.Gravity");

    //Controls //temp default values
    controls.moveForward = OIS::KC_W;
    controls.moveLeft = OIS::KC_A;
    controls.moveBack = OIS::KC_S;
    controls.moveRight = OIS::KC_D;

    controls.jump = OIS::KC_SPACE;
    controls.kick = OIS::KC_F;
    controls.run = OIS::KC_LSHIFT;
    controls.leftHand = OIS::MB_Left;
    controls.rightHand = OIS::MB_Right;

    controls.quickslots.push_back(OIS::KC_1);
    controls.quickslots.push_back(OIS::KC_2);
    controls.quickslots.push_back(OIS::KC_3);
    controls.quickslots.push_back(OIS::KC_4);
    controls.quickslots.push_back(OIS::KC_5);
    controls.quickslots.push_back(OIS::KC_6);
    controls.quickslots.push_back(OIS::KC_7);
    controls.quickslots.push_back(OIS::KC_8);
    controls.quickslots.push_back(OIS::KC_9);
    controls.quickslots.push_back(OIS::KC_0);

    controls.exit = OIS::KC_ESCAPE;

    controls.freezeCollision = OIS::KC_Q;
    controls.addItem = OIS::KC_1;
    controls.addMonster = OIS::KC_2;
    controls.reset = OIS::KC_3;

    controls.console = OIS::KC_GRAVE;

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
    dataFiles.push_back(pt.get<std::string>("Scenes.Data"));
    dataFiles.push_back(pt.get<std::string>("Architecture.Data"));
    dataFiles.push_back(pt.get<std::string>("Monsters.Data"));
    dataFiles.push_back(pt.get<std::string>("Items.Data"));
    dataFiles.push_back(pt.get<std::string>("Sounds.Data"));

    //Debug
    debug = (pt.get<std::string>("Debug.DebugMode") == TRUE_STRING);
    verbose = (pt.get<std::string>("Debug.VerboseMode") == TRUE_STRING);
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
  catch(boost::property_tree::ini_parser::ini_parser_error e)
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
bool World::isDebug()
{
  return debug;
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


