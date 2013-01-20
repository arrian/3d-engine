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
#include "ArchitectureManager.h"

//-------------------------------------------------------------------------------------
Scene::Scene(SceneDesc desc, World* world)
  : world(world),
    desc(desc),
    sceneGraphicsManager(NULL),
    sceneControllerManager(NULL),
    scenePhysicsManager(NULL),
    defaultEntry(NULL),
    localPlayer(NULL),
    numberPhysicsCPUThreads(4),
    stepSize(1.0 / 60.0),
    accumulator(0.0),
    scenePathfindManager(NULL),
    particles(),
    monsters(),
    portals(),
    lights(),
    items(),
    players()
{
  setup();
  
  ivy = new Ivy(this, Vector3(0,-2,5), Vector3(0,-1,0));
  addInteractive(0);//temp door
}

//-------------------------------------------------------------------------------------
Scene::~Scene(void)
{
  delete ivy;
  release();
}

//-------------------------------------------------------------------------------------
std::string Scene::getName()
{
  return desc.name;
}

//-------------------------------------------------------------------------------------
Ogre::SceneManager* Scene::getGraphicsManager()
{
  return sceneGraphicsManager;
}

//-------------------------------------------------------------------------------------
physx::PxScene* Scene::getPhysicsManager()
{
  return scenePhysicsManager;
}

//-------------------------------------------------------------------------------------
physx::PxControllerManager* Scene::getControllerManager()
{
  return sceneControllerManager;
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
  monster->setGoal(new Go(getPathfindManager()->getRandomNavigablePoint(), Priority::HIGH));
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
void Scene::addParticles(Ogre::String name, Ogre::String templateName, Vector3 position, Ogre::Real speed)
{
  Ogre::ParticleSystem* particle = sceneGraphicsManager->createParticleSystem(name, templateName);//"Rain");//, "Examples/Rain");
  particle->setSpeedFactor(speed);
  Ogre::SceneNode* particleNode = sceneGraphicsManager->getRootSceneNode()->createChildSceneNode();//"particle" + Ogre::StringConverter::toString(getNewInstanceNumber()));
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
void Scene::removeItem(Item* item)
{
  items.destroy(item);
}

//-------------------------------------------------------------------------------------
void Scene::removeMonster(Monster* monster)
{
  monsters.destroy(monster);
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

  if(world->isPhysicsEnabled()) advancePhysics(elapsedSeconds);

  //post simulation events
}

//-------------------------------------------------------------------------------------
bool Scene::advancePhysics(double elapsedSeconds)
{
  accumulator += elapsedSeconds;
  if(accumulator < stepSize) return false;
  accumulator -= stepSize;
  scenePhysicsManager->simulate((float) stepSize);
  scenePhysicsManager->fetchResults(true);

  return true;
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
Portal* Scene::getDefaultPortal()
{
  return getPortal(0);//could make more intelligent
}

//-------------------------------------------------------------------------------------
void Scene::destroyAllAttachedMoveables(Ogre::SceneNode* node)
{
  if(!node) return;

  // Destroy all the attached objects
  Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

  while (itObject.hasMoreElements()) node->getCreator()->destroyMovableObject(itObject.getNext());

  // Recurse to child SceneNodes
  Ogre::SceneNode::ChildNodeIterator itChild = node->getChildIterator();

  while (itChild.hasMoreElements())
  {
    Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
    destroyAllAttachedMoveables(pChildNode);
  }
}

//-------------------------------------------------------------------------------------
void Scene::destroySceneNode(Ogre::SceneNode* node)
{
  if(!node) return;
  destroyAllAttachedMoveables(node);
  node->removeAndDestroyAllChildren();
  node->getCreator()->destroySceneNode(node);
}

//-------------------------------------------------------------------------------------
ArchitectureManager* Scene::getArchitectureManager()
{
  return sceneArchitectureManager;
}

//-------------------------------------------------------------------------------------
PathfindManager* Scene::getPathfindManager()
{
  return scenePathfindManager;
}

//-------------------------------------------------------------------------------------
void Scene::setShadowsEnabled(bool enabled)
{
  if(enabled) 
  {
    sceneGraphicsManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
    sceneGraphicsManager->setShadowColour(desc.shadowColour);
    sceneGraphicsManager->setShadowTextureSize(4096);//1024 seems very pixelated
    sceneGraphicsManager->setShadowTextureCount(5);
  }
  else
  {
    sceneGraphicsManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
  }
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawShadows(bool enabled)
{
  sceneGraphicsManager->setShowDebugShadows(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawBoundingBoxes(bool enabled)
{
  sceneGraphicsManager->showBoundingBoxes(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawNavigationMesh(bool enabled)
{
  scenePathfindManager->setDrawNavigationMesh(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setGravity(Vector3 gravity)
{
  scenePhysicsManager->setGravity(physx::PxVec3(gravity.x, gravity.y, gravity.z));
}

//-------------------------------------------------------------------------------------
Vector3 Scene::getGravity()
{
  physx::PxVec3 gravity = scenePhysicsManager->getGravity();
  return Vector3(gravity.x, gravity.y, gravity.z);
}

//-------------------------------------------------------------------------------------
void Scene::reset()
{
  release();
  setup();
}

//-------------------------------------------------------------------------------------
void Scene::setup()
{
  //static physx::PxDefaultSimulationFilterShader defaultFilterShader;//??

  //scene physics
  physx::PxSceneDesc physicsDesc(world->getPhysicsManager()->getTolerancesScale());
  physicsDesc.gravity = physx::PxVec3(desc.gravity.x, desc.gravity.y, desc.gravity.z);

  physicsDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(numberPhysicsCPUThreads);
  if(!physicsDesc.cpuDispatcher) throw NHException("could not create scene CPU dispatcher");

  physicsDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
  if(!physicsDesc.filterShader) throw NHException("filter shader creation failed");

  scenePhysicsManager = world->getPhysicsManager()->getPhysics()->createScene(physicsDesc);
  if(!scenePhysicsManager) throw NHException("could not create scene physics manager");

  sceneControllerManager = PxCreateControllerManager(*world->getPhysicsManager()->getFoundation());
  if(!sceneControllerManager) throw NHException("could not create scene controller manager");

  sceneGraphicsManager = world->getRoot()->createSceneManager(Ogre::ST_GENERIC);

  scenePathfindManager = new PathfindManager(sceneGraphicsManager);

  setShadowsEnabled(world->isShadowsEnabled());

  sceneArchitectureManager = new ArchitectureManager(this, scenePathfindManager);

  setAmbientColour(desc.ambientLight);

  SceneLoader().load(desc.file, this);//load an xml scene

  //flockTest.setScene(this);//boids flocking test

  if(localPlayer) addPlayer(localPlayer);


#ifdef _DEBUG
  scenePhysicsManager->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
  scenePhysicsManager->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
#endif
}

//-------------------------------------------------------------------------------------
void Scene::release()
{
  if(localPlayer) localPlayer->setScene(NULL);//detach player from this scene
  //localplayer to NULL?

  defaultEntry = NULL;

  delete scenePathfindManager;
  scenePathfindManager = NULL;

  delete sceneArchitectureManager;
  sceneArchitectureManager = NULL;

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
  
  if(scenePhysicsManager) scenePhysicsManager->release();
  scenePhysicsManager = NULL;

  if(sceneGraphicsManager) world->getRoot()->destroySceneManager(sceneGraphicsManager);
  sceneGraphicsManager = NULL;
}

//-------------------------------------------------------------------------------------
bool Scene::hasPlayer()
{
  return localPlayer != NULL;
}

//-------------------------------------------------------------------------------------
void Scene::setAmbientColour(Ogre::ColourValue colour)
{
  sceneGraphicsManager->setAmbientLight(colour);
}