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
Scene::Scene(SceneDesc desc, World* world)
  : world(world),
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
  setup();
}

//-------------------------------------------------------------------------------------
Scene::~Scene(void)
{
}

//-------------------------------------------------------------------------------------
void Scene::setup()
{
  scenePhysicsManager = std::shared_ptr<ScenePhysicsManager>(new ScenePhysicsManager(world->getPhysicsManager(), desc.gravity));
  sceneGraphicsManager = std::shared_ptr<SceneGraphicsManager>(new SceneGraphicsManager(world->getGraphicsManager(), desc.ambientLight, desc.shadowColour));
  scenePathfindManager = std::shared_ptr<ScenePathfindManager>(new ScenePathfindManager(sceneGraphicsManager->getSceneManager()));
  sceneArchitectureManager = std::shared_ptr<SceneArchitectureManager>(new SceneArchitectureManager(this));

  SceneLoader::load(desc.file, this);//load an xml scene

  //Dev
  //flockTest.setScene(this);//boids flocking test
  //ivy = new Ivy(this, Vector3(0,-2,5), Vector3(0,-1,0));
  //add(std::shared_ptr<Interactive>(new Door()));//temp door
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
  std::shared_ptr<Item> item(new Item(world->getDataManager()->get<ItemDesc>(dataId)));
  item->setScene(this);
  item->setPosition(position);
  item->setRotation(rotation);
  items.insert(instanceId, item);
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Creature> Scene::addCreature(int dataId, Vector3 position, Vector3 lookAt, Id<Creature> instanceId)//add an object to the scene
{
  std::shared_ptr<Creature> creature(new Creature(world->getDataManager()->get<CreatureDesc>(dataId)));
  creature->setScene(this);
  creature->setPosition(position);
  creature->setLookAt(lookAt);
  creatures.insert(instanceId, creature);
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Effect> Scene::addLight(int dataId, Vector3 position, bool castShadows, float range, Id<Effect> instanceId)//add an object to the scene
{
  Light* light(new Light(world->getDataManager()->get<LightDesc>(dataId)));
  light->setScene(this);
  light->setPosition(position);
  effects.insert(instanceId, std::shared_ptr<Effect>(light));
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Interactive> Scene::addInteractive(int dataId, Vector3 position, Id<Interactive> instanceId)//add an object to the scene
{
  std::shared_ptr<Interactive> interactive(new Interactive(world->getDataManager()->get<InteractiveDesc>(dataId)));
  interactive->setScene(this);
  interactive->setPosition(position);
  interactives.insert(instanceId, interactive);
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Portal> Scene::addPortal(int dataId, Vector3 position, Vector3 lookAt, Id<Scene> targetScene, Id<Portal> targetPortal, Id<Portal> instanceId)//add an object to the scene
{
  std::shared_ptr<Portal> portal(new Portal(world->getDataManager()->get<PortalDesc>(dataId), targetScene, targetPortal));
  portal->setScene(this);
  portal->setPosition(position);
  portal->setLookAt(lookAt);
  portals.insert(instanceId, portal);
  return instanceId;
}

//-------------------------------------------------------------------------------------
Id<Player> Scene::addPlayer(std::shared_ptr<Player> player, Vector3 position, Vector3 lookAt, Id<Player> instanceId)//add an object to the scene
{
  Vector3 lookAt = Vector3::UNIT_Z;

  Scene* old = player->getScene();
  if(old) old->remove<Player>(player.get());//need to remove player from previous scene
  player->setScene(this);
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
  sceneArchitectureManager->add(instanceId, std::shared_ptr<Architecture>(new Architecture(world->getDataManager()->get<ArchitectureDesc>(dataId), position, rotation, scale)));
  return instanceId;
}



