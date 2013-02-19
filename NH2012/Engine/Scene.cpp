#include "Scene.h"

#include <algorithm>

#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSimpleFactory.h"
#include "PxVisualizationParameter.h"

#include "World.h"
#include "Player.h."
#include "Monster.h"
#include "Item.h"
#include "Light.h"
#include "Portal.h"
#include "Flock.h"
#include "Door.h"
#include "Chest.h"
#include "Ivy.h"
#include "SceneLoader.h"

#include "SceneArchitectureManager.h"
#include "ScenePathfindManager.h"
#include "SceneGraphicsManager.h"
#include "ScenePhysicsManager.h"

//-------------------------------------------------------------------------------------
Scene::Scene(SceneDesc desc, World* world)
  : world(world),
    desc(desc),
    scenePathfindManager(NULL),
    sceneGraphicsManager(NULL),
    scenePhysicsManager(NULL),
    sceneArchitectureManager(NULL),
    defaultEntry(NULL),
    localPlayer(NULL),
    particles(),
    monsters(),
    portals(),
    lights(),
    items(),
    players()
{
  setup();
}

//-------------------------------------------------------------------------------------
Scene::~Scene(void)
{
  release();
}

//-------------------------------------------------------------------------------------
void Scene::setup()
{
  scenePhysicsManager = new ScenePhysicsManager(world->getPhysicsManager(), desc.gravity);
  sceneGraphicsManager = new SceneGraphicsManager(world->getGraphicsManager(), desc.ambientLight, desc.shadowColour);//world->getGraphicsManager()->createSceneGraphicsManager();//->getRoot()->createSceneManager(Ogre::ST_GENERIC);
  scenePathfindManager = new ScenePathfindManager(sceneGraphicsManager->getSceneManager());
  sceneArchitectureManager = new SceneArchitectureManager(this);

  SceneLoader().load(desc.file, this);//load an xml scene
  if(localPlayer) addPlayer(localPlayer);

  //Dev
  //flockTest.setScene(this);//boids flocking test
  //ivy = new Ivy(this, Vector3(0,-2,5), Vector3(0,-1,0));
  addInteractive(0);//temp door
}

//-------------------------------------------------------------------------------------
void Scene::release()
{
  //delete ivy;

  if(localPlayer) localPlayer->setScene(NULL);//detach player from this scene
  //localplayer to NULL?

  defaultEntry = NULL;

  lights.destroyAll();
  monsters.destroyAll();
  items.destroyAll();
  interactives.destroyAll();
  players.destroyAll();

  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it)
  {
    delete (*it);
  }
  portals.clear();

  delete scenePathfindManager;
  scenePathfindManager = NULL;

  delete sceneArchitectureManager;
  sceneArchitectureManager = NULL;

  delete sceneGraphicsManager;
  sceneGraphicsManager = NULL;

  delete scenePhysicsManager;
  scenePhysicsManager = NULL;
}

//-------------------------------------------------------------------------------------
void Scene::reset()
{
  release();
  setup();
}

//-------------------------------------------------------------------------------------
void Scene::update(double elapsedSeconds)
{

  //ivy->update(elapsedSeconds);

  scenePathfindManager->update(elapsedSeconds);//for nav mesh updates

  lights.update(elapsedSeconds);
  monsters.update(elapsedSeconds);
  items.update(elapsedSeconds);
  interactives.update(elapsedSeconds);
  players.update(elapsedSeconds);

  if(localPlayer) localPlayer->update(elapsedSeconds);

  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it) 
  {
    if((*it)->isLoadRequired(localPlayer->getPosition())) world->loadScene((*it)->getID());
  }

  //pre simulation updates

  scenePhysicsManager->update(elapsedSeconds);

  //if(world->getPhysicsManager()->isEnabled()) advancePhysics(elapsedSeconds);

  //post simulation events
}

//-------------------------------------------------------------------------------------
void Scene::addPlayer(Player* player, int portalID)
{
  Vector3 position;
  Vector3 lookAt;
  if(defaultEntry != NULL && portalID == DEFAULT_PORTAL)//use default portal
  {
    position = defaultEntry->getPosition();
    lookAt = defaultEntry->getLookAt();
  }
  else if(portalID >= 0)//use specified portal
  {
    Portal* portal = getPortal(portalID);
    position = portal->getPosition();
    lookAt = portal->getLookAt();
  }
  else//place the player at zero
  {
    position = Vector3::ZERO;
    lookAt = Vector3::UNIT_Z;
  }

  Scene* old = player->getScene();
  if(old) old->removePlayer(player);//need to remove player from previous scene
  player->setScene(this);
  player->setPosition(position);
  player->setLookAt(lookAt);
  this->localPlayer = player;
}

//-------------------------------------------------------------------------------------
void Scene::addMonster(int id, Vector3 position, Quaternion rotation)
{
  Monster* monster = new Monster(world->getDataManager()->getMonster(id));
  monster->setScene(this);
  monster->setPosition(position);
  monster->setGoal(new Go(scenePathfindManager->getRandomNavigablePoint(), Priority::HIGH));
  monsters.add(monster);
}

//-------------------------------------------------------------------------------------
void Scene::addItem(int id, Vector3 position, Quaternion rotation)
{
  Item* item = new Item(world->getDataManager()->getItem(id));//this->getWorld()->createItem(id);
  item->setScene(this);
  item->setPosition(position);
  item->setRotation(rotation);
  items.add(item);
}

//-------------------------------------------------------------------------------------
void Scene::addInteractive(int id, Vector3 position, Quaternion rotation)
{
  Interactive* interactive = NULL;
  if(id == 0) interactive = new Door();
  else if(id == 1) interactive = new Chest();
  else throw NHException("the specified interactive object id has not been implemented");

  interactive->setScene(this);
  interactive->setPosition(position);
  interactive->setRotation(rotation);
  interactives.add(interactive);
}

//-------------------------------------------------------------------------------------
void Scene::addLight(Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour)
{
  Light* light = new Light(this, position, castShadows, range, colour);
  lights.add(light);
}

//-------------------------------------------------------------------------------------
void Scene::addArchitecture(int id, Vector3 position, Quaternion quaternion, Vector3 scale)
{
  sceneArchitectureManager->add(world->getDataManager()->getArchitecture(id), position, quaternion, scale);
}

//-------------------------------------------------------------------------------------
void Scene::addParticles(std::string name, std::string templateName, Vector3 position, Ogre::Real speed)
{
  Ogre::ParticleSystem* particle = sceneGraphicsManager->createParticleSystem(name, templateName);//"Rain");//, "Examples/Rain");
  particle->setSpeedFactor(speed);
  Ogre::SceneNode* particleNode = sceneGraphicsManager->createSceneNode();//"particle" + Ogre::StringConverter::toString(getNewInstanceNumber()));
  particleNode->setPosition(position);
  particleNode->attachObject(particle);
  particles.push_back(particle);
}

//-------------------------------------------------------------------------------------
void Scene::addPortal(Portal* portal)
{
  portals.push_back(portal);
  if(defaultEntry == NULL) defaultEntry = portals[0];//creating a default entry point
}

//-------------------------------------------------------------------------------------
void Scene::removePlayer(Player* player)
{
  if(this->localPlayer == player) this->localPlayer = NULL;
}

//-------------------------------------------------------------------------------------
void Scene::destroyItem(Item* item)
{
  items.destroy(item);
}

//-------------------------------------------------------------------------------------
void Scene::destroyMonster(Monster* monster)
{
  monsters.destroy(monster);
}

//-------------------------------------------------------------------------------------
void Scene::removeItem(Item* item)
{
  items.remove(item);
}

//-------------------------------------------------------------------------------------
void Scene::removeMonster(Monster* monster)
{
  monsters.remove(monster);
}

//-------------------------------------------------------------------------------------
void Scene::build()
{
  scenePathfindManager->build();//note that this needs to be done before building architecture because architecture->build destroys the required scenenode
  sceneArchitectureManager->build();
}

//-------------------------------------------------------------------------------------
World* Scene::getWorld()
{
  return world;
}

//-------------------------------------------------------------------------------------
std::string Scene::getName()
{
  return desc.name;
}

//-------------------------------------------------------------------------------------
int Scene::getSceneID()
{
  return desc.id;
}

//-------------------------------------------------------------------------------------
Portal* Scene::getPortal(int id)
{
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it)
  {
    if((*it)->getID() == id) return (*it);
  }

  throw NHException("no portal was found with the given id in Scene::getPortal()");//temporarily throw exception
  return NULL;
}

//-------------------------------------------------------------------------------------
bool Scene::hasPlayer()
{
  return localPlayer != NULL;
}

