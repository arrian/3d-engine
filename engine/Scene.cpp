#include "Scene.h"

#include <algorithm>

#include <OgreEntity.h>
#include <OgreColourValue.h>
#include <OgreSceneManager.h>

#include "World.h"
#include "Player.h."
#include "Creature.h"
#include "Item.h"
#include "Light.h"
#include "Portal.h"
#include "Flock.h"
#include "Door.h"
#include "Chest.h"
#include "Ivy.h"
#include "Effect.h"
#include "SceneLoader.h"
#include "Interactive.h"

#include "SceneArchitectureManager.h"
#include "ScenePathfindManager.h"
#include "SceneGraphicsManager.h"
#include "ScenePhysicsManager.h"

//-------------------------------------------------------------------------------------
Scene::Scene(SceneDesc desc, boost::shared_ptr<World> world)
  : world(boost::weak_ptr<World>(world)),
    desc(desc),
    scenePathfindManager(),
    sceneGraphicsManager(),
    scenePhysicsManager(),
    sceneArchitectureManager(),
    creatures(),
    portals(),
    items(),
    players(),
    effects(),
    interactives(),
    localPlayer(0),
    defaultPortal(0)
{
}

//-------------------------------------------------------------------------------------
Scene::~Scene(void)
{
  //need to destruct all scene objects before scene and world references destroyed

  scenePhysicsManager.reset();
  sceneGraphicsManager.reset();
  scenePathfindManager.reset();
  sceneArchitectureManager.reset();

  world = boost::weak_ptr<World>();
}

//-------------------------------------------------------------------------------------
void Scene::initialise()
{
  scenePhysicsManager = boost::shared_ptr<ScenePhysicsManager>(new ScenePhysicsManager(shared_from_this(), desc.gravity));//world->getPhysicsManager()
  sceneGraphicsManager = boost::shared_ptr<SceneGraphicsManager>(new SceneGraphicsManager(shared_from_this(), desc.ambientLight, desc.shadowColour));//world->getGraphicsManager()
  scenePathfindManager = boost::shared_ptr<ScenePathfindManager>(new ScenePathfindManager(shared_from_this()));//sceneGraphicsManager->getSceneManager()
  sceneArchitectureManager = boost::shared_ptr<SceneArchitectureManager>(new SceneArchitectureManager(shared_from_this()));

  SceneLoader::load(desc.file, this);//load an xml scene

  //Dev
  //flockTest.setScene(this);//boids flocking test
  //ivy = new Ivy(this, Vector3(0,-2,5), Vector3(0,-1,0));
  //add(std::shared_ptr<Interactive>(new Door()));//temp door
}

//-------------------------------------------------------------------------------------
void Scene::reset()
{
  boost::shared_ptr<Player> player(players.remove(localPlayer));

  items.clear();
  players.clear();
  portals.clear();
  creatures.clear();
  effects.clear();
  interactives.clear();

  scenePhysicsManager.reset();
  sceneGraphicsManager.reset();
  scenePathfindManager.reset();
  sceneArchitectureManager.reset();
  
  initialise();

  addPlayer(player, getDefaultPortal()->getPosition(), getDefaultPortal()->getLookAt());
}

//-------------------------------------------------------------------------------------
void Scene::update(double elapsedSeconds)
{
  scenePathfindManager->update(elapsedSeconds);//for nav mesh updates

  for(Container<Item>::Iterator it = items.begin(); it != items.end(); ++it) it->second->update(elapsedSeconds);
  for(Container<Creature>::Iterator it = creatures.begin(); it != creatures.end(); ++it) it->second->update(elapsedSeconds);
  for(Container<Player>::Iterator it = players.begin(); it != players.end(); ++it) it->second->update(elapsedSeconds);
  for(Container<Interactive>::Iterator it = interactives.begin(); it != interactives.end(); ++it) it->second->update(elapsedSeconds);
  for(Container<Portal>::Iterator it = portals.begin(); it != portals.end(); ++it) it->second->update(elapsedSeconds);
  for(Container<Effect>::Iterator it = effects.begin(); it != effects.end(); ++it) it->second->update(elapsedSeconds);

  //Pre-simulation updates

  scenePhysicsManager->update(elapsedSeconds);

  //Post-simulation events
}

//-------------------------------------------------------------------------------------
void Scene::build()
{
  scenePathfindManager->build();//note that this needs to be done before building architecture because architecture->build destroys the required scenenode
  sceneArchitectureManager->build();
}

//-------------------------------------------------------------------------------------
Id<Item> Scene::addItem(int dataId, Vector3 position, Quaternion rotation, Id<Item> instanceId)//add an object to the scene
{
  boost::shared_ptr<World> world = getWorld();
  if(!world) throw NHException("Attempting to add item to scene failed. World has expired.");
  
  boost::shared_ptr<Item> item(new Item(world->getDataManager()->get<ItemDesc>(dataId)));
  item->setScene(shared_from_this());
  item->setPosition(position);
  item->setRotation(rotation);
  items.insert(instanceId, item);
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Creature> Scene::addCreature(int dataId, Vector3 position, Vector3 lookAt, Id<Creature> instanceId)//add an object to the scene
{
  boost::shared_ptr<World> world = getWorld();
  if(!world) throw NHException("Attempting to add creature to scene failed. World has expired.");

  boost::shared_ptr<Creature> creature(new Creature(world->getDataManager()->get<CreatureDesc>(dataId)));
  creature->setScene(shared_from_this());
  creature->setPosition(position);
  creature->setLookAt(lookAt);
  creatures.insert(instanceId, creature);
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Effect> Scene::addLight(int dataId, Vector3 position, bool castShadows, float range, Id<Effect> instanceId)//add an object to the scene
{
  boost::shared_ptr<World> world = getWorld();
  if(!world) throw NHException("Attempting to add light to scene failed. World has expired.");

  LightDesc desc(world->getDataManager()->get<LightDesc>(dataId));
  desc.range = range;
  desc.castShadows = castShadows;
  Light* light(new Light(desc));
  light->setScene(shared_from_this());
  light->setPosition(position);
  effects.insert(instanceId, boost::shared_ptr<Effect>(light));
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Interactive> Scene::addInteractive(int dataId, Vector3 position, Id<Interactive> instanceId)//add an object to the scene
{
  boost::shared_ptr<World> world = getWorld();
  if(!world) throw NHException("Attempting to add interactive to scene failed. World has expired.");

  boost::shared_ptr<Interactive> interactive(new Interactive(world->getDataManager()->get<InteractiveDesc>(dataId)));
  interactive->setScene(shared_from_this());
  interactive->setPosition(position);
  interactives.insert(instanceId, interactive);
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Portal> Scene::addPortal(int dataId, Vector3 position, Vector3 lookAt, Id<Scene> targetScene, Id<Portal> targetPortal, Id<Portal> instanceId)//add an object to the scene
{
  boost::shared_ptr<World> world = getWorld();
  if(!world) throw NHException("Attempting to add portal to scene failed. World has expired.");

  boost::shared_ptr<Portal> portal(new Portal(world->getDataManager()->get<PortalDesc>(dataId), targetScene, targetPortal));
  portal->setScene(this);
  portal->setPosition(position);
  portal->setLookAt(lookAt);
  portals.insert(instanceId, portal);
  if(!defaultPortal.isValid()) defaultPortal = instanceId;
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Player> Scene::addPlayer(boost::shared_ptr<Player> player, Vector3 position, Vector3 lookAt, Id<Player> instanceId)//add an object to the scene
{
  boost::shared_ptr<World> world = getWorld();
  if(!world) throw NHException("Attempting to add player to scene failed. World has expired.");

  boost::shared_ptr<Scene> old = player->getScene();
  if(old) old->remove<Player>(player.get());//need to remove player from previous scene
  player->setScene(shared_from_this());
  player->setGravity(getScenePhysicsManager()->getGravity());
  player->setLookAt(lookAt);

  if(player->isLocalPlayer())
  {
    world->getGraphicsManager()->setCamera(player->getCamera());
    this->localPlayer = instanceId;
  }
  players.insert(instanceId, player);
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Architecture> Scene::addArchitecture(int dataId, Vector3 position, Quaternion rotation, Vector3 scale, Id<Architecture> instanceId)
{
  boost::shared_ptr<World> world = getWorld();
  if(!world) throw NHException("Attempting to add architecture to scene failed. World has expired.");

  sceneArchitectureManager->add(instanceId, boost::shared_ptr<Architecture>(new Architecture(world->getDataManager()->get<ArchitectureDesc>(dataId), position, rotation, scale)));
  return instanceId;
}



