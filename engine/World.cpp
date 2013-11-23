
#include "World.h"

//Boost
#include <boost/lexical_cast.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

//Local
#include "Scene.h"
#include "Player.h"
#include "Item.h"
#include "Creature.h"
#include "ScriptManager.h"
#include "ControlMap.h"
#include "NHException.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "InitialisationParser.h"
#include "ScenePhysicsManager.h"
#include "WorldLoader.h"
#include "Portal.h"



//-------------------------------------------------------------------------------------
World::World()
  : //scenes(),
    scene(),
    timeManager(),
    dataManager(),
    soundManager(),
    scriptManager(),
    physicsManager(),
    controlManager(),
    networkManager(),
    graphicsManager(),
    player(),
    defaultScene(0),
    playerId(0)
{
}

//-------------------------------------------------------------------------------------
World::World(boost::shared_ptr<Ogre::Root> root, Ogre::RenderWindow* window)
  : //scenes(),
    scene(),
    timeManager(),
    dataManager(),
    soundManager(),
    scriptManager(),
    physicsManager(),
    controlManager(),
    networkManager(),
    graphicsManager(),
    player(),
    defaultScene(0),
    playerId(0)
{
  std::cout << "before: " << root.use_count() << std::endl;//TODO: remove
  graphicsManager.setRoot(root);
  std::cout << "after: " << root.use_count() << std::endl;//TODO: remove
  graphicsManager.hookWindow(window);
}

//-------------------------------------------------------------------------------------
World::~World(void)
{
}

//-------------------------------------------------------------------------------------
void World::initialise(std::string iniFile)
{
  scriptManager.setWorld(shared_from_this());

  WorldLoader::load(iniFile, this);

  //Create default scene
  loadScene(defaultScene);
  if(!scene) throw NHException("Default scene creation failed. Unable to load the scene.");
  
  //Create default player
  loadPlayer(Id<Player>());
  if(!player) throw NHException("Default player creation failed. Unable to load the player.");

  //Add player to scene
  boost::shared_ptr<Portal> portal = scene->getDefaultPortal();
  scene->addPlayer(player, portal->getPosition(), portal->getLookAt(), playerId);
}

//-------------------------------------------------------------------------------------
bool World::update(double elapsedSeconds)
{
  if(!scriptManager.update(elapsedSeconds)) return false;
  timeManager.update(elapsedSeconds);
  //for(Container<Scene>::Iterator it = scenes.begin(); it != scenes.end(); ++it) it->second->update(elapsedSeconds);
  scene->update(elapsedSeconds);
  return true;
}

//-------------------------------------------------------------------------------------
boost::shared_ptr<Player> World::getPlayer()
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
SoundManager* World::getSoundManager()
{
  return &soundManager;
}

//-------------------------------------------------------------------------------------
boost::shared_ptr<Scene> World::getScene() 
{
  return scene;
}

/*
//-------------------------------------------------------------------------------------
boost::shared_ptr<Scene> World::getScene(Id<Scene> id)
{
  if(scene->getId() == id) return scene;
  return boost::shared_ptr<Scene>();
  //return scenes.get(id);
}*/

//-------------------------------------------------------------------------------------
bool World::hasScene(Id<Scene> id)
{
  return scene->getId() == id;
  //return scenes.contains(id);
}

//-------------------------------------------------------------------------------------
int World::getSceneCount()
{
  if(scene) return 1;
  return 0;
  //return scenes.count();
}

//-------------------------------------------------------------------------------------
void World::getSceneNames(std::map<Id<Scene>, std::string> &names)
{
  names.insert(std::pair<Id<Scene>, std::string>(scene->getId(), scene->getName()));
  //for(Container<Scene>::Iterator it = scenes.begin(); it != scenes.end(); ++it) 
  //{
  //  names.insert(std::pair<Id<Scene>, std::string>(it->first, it->second->getName()));
  //}
}

//-------------------------------------------------------------------------------------
boost::shared_ptr<Scene> World::loadScene(Id<Scene> id)
{
  //if(scenes.contains(id)) return scenes.get(id);//check that the scene has not been loaded already
  if(scene && scene->getId() == id) return scene;

  //std::shared_ptr<Scene> scene(new Scene(dataManager.get<SceneDesc>(id.getInstance()), shared_from_this()));
  //scenes.insert(id, scene);
  //return scene.get();

  boost::shared_ptr<Scene> scene(new Scene(dataManager.get<SceneDesc>(id.getInstance()), shared_from_this()));
  scene->initialise();
  this->scene = scene;
  return scene;
}

//-------------------------------------------------------------------------------------
boost::shared_ptr<Player> World::loadPlayer(Id<Player> id)
{
  if(playerId == id) return player;
  
  //Create default player - TODO: load player from file //dataManager.get<PlayerDesc>(id.getInstance());
  PlayerDesc playerDesc = PlayerDesc();
  playerId = id;
  player = boost::shared_ptr<Player>(new Player(playerDesc, shared_from_this()));
  return player;
}

//-------------------------------------------------------------------------------------
void World::destroyScene(Id<Scene> id)
{
  //Scene* scene = scenes.get(id);
  
  if(scene->getId() != id) throw NHException("there is no scene loaded with that id");
  
  if(scene->contains<Player>(playerId)) throw NHException("unable to destroy a scene while it contains the local player");
  scene.reset();
}

//-------------------------------------------------------------------------------------
void World::keyPressed(const OIS::KeyEvent &arg)
{
  if(player) player->keyPressed(arg);
}

//-------------------------------------------------------------------------------------
void World::keyReleased(const OIS::KeyEvent &arg)
{
  if(player) player->keyReleased(arg);
}

//-------------------------------------------------------------------------------------
void World::mouseMoved(const OIS::MouseEvent &arg)
{
  if(player) player->mouseMoved(arg);
}

//-------------------------------------------------------------------------------------
void World::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  if(player) player->mousePressed(arg, id);
}

//-------------------------------------------------------------------------------------
void World::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  if(player) player->mouseReleased(arg, id);
}

//-------------------------------------------------------------------------------------
void World::setPlayerScene(Id<Scene> sceneId, Vector3 position, Vector3 lookAt) 
{
  getScene(sceneId)->addPlayer(player, position, lookAt);
}



