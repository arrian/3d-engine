
#include "World.h"

//Boost
#include <boost/lexical_cast.hpp>

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
  : scenes(),
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
  scriptManager.setWorld(this);
}

//-------------------------------------------------------------------------------------
World::World(Ogre::Root* root, Ogre::RenderWindow* window)
  : scenes(),
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
  scriptManager.setWorld(this);
  graphicsManager.setRoot(root);
  graphicsManager.hookWindow(window);
}

//-------------------------------------------------------------------------------------
World::~World(void)
{
}

//-------------------------------------------------------------------------------------
void World::initialise(std::string iniFile)
{
  WorldLoader::load(iniFile, this);

  //Create default scene
  Scene* scene = loadScene(defaultScene);
  if(!scene) throw NHException("default scene creation failed");
  
  //Create default player
  loadPlayer(Id<Player>());
  if(!player) throw NHException("default player creation failed");

  //Add player to scene
  Portal* portal = scene->getDefaultPortal();
  scene->addPlayer(player, portal->getPosition(), portal->getLookAt(), playerId);
}

//-------------------------------------------------------------------------------------
bool World::update(double elapsedSeconds)
{
  if(!scriptManager.update(elapsedSeconds)) return false;
  timeManager.update(elapsedSeconds);
  for(Container<Scene>::Iterator it = scenes.begin(); it != scenes.end(); ++it) it->second->update(elapsedSeconds);
  return true;
}

//-------------------------------------------------------------------------------------
Player* World::getPlayer()
{
  return player.get();
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
Scene* World::getScene(Id<Scene> id)
{
  return scenes.get(id);
}

//-------------------------------------------------------------------------------------
bool World::hasScene(Id<Scene> id)
{
  return scenes.contains(id);
}

//-------------------------------------------------------------------------------------
int World::getSceneCount()
{
  return scenes.count();
}

//-------------------------------------------------------------------------------------
void World::getSceneNames(std::map<Id<Scene>, std::string> &names)
{
  for(Container<Scene>::Iterator it = scenes.begin(); it != scenes.end(); ++it) 
  {
    names.insert(std::pair<Id<Scene>, std::string>(it->first, it->second->getName()));
  }
}

//-------------------------------------------------------------------------------------
Scene* World::loadScene(Id<Scene> id)
{
  if(scenes.contains(id)) return scenes.get(id);//check that the scene has not been loaded already

  std::shared_ptr<Scene> scene(new Scene(dataManager.get<SceneDesc>(id.getInstance()), this));
  scenes.insert(id, scene);
  return scene.get();
}

//-------------------------------------------------------------------------------------
Player* World::loadPlayer(Id<Player> id)
{
  if(playerId == id) return player.get();
  
  //Create default player - TODO: load player from file //dataManager.get<PlayerDesc>(id.getInstance());
  PlayerDesc playerDesc = PlayerDesc();
  playerId = id;
  player = std::shared_ptr<Player>(new Player(playerDesc, this));
  return player.get();
}

//-------------------------------------------------------------------------------------
void World::destroyScene(Id<Scene> id)
{
  Scene* scene = scenes.get(id);
  if(!scene) throw NHException("there is no scene loaded with that id");
  
  if(scene->contains<Player>(playerId)) throw NHException("unable to destroy a scene while it contains the local player");
  scenes.remove(id);
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
  if(player) getScene(sceneId)->addPlayer(player,position,lookAt);
}



