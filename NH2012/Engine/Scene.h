#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "DataManager.h"
#include "PathfindManager.h"
#include "SceneContainer.h"
#include "Flock.h"
#include "Vector3.h"
#include "Quaternion.h"

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"

//statics
static const int DEFAULT_PORTAL = -1;

//forward declarations
class World;
class Player;
class Monster;
class Portal;
class Item;
class Light;
class Flock;
class BoidExtension;
class ArchitectureManager;
class Interactive;
class Ivy;

class Scene
{
public:
  Scene(SceneDesc desc, World* world);
  ~Scene(void);

  //setters
  void setGravity(Vector3 gravity);
  void setAmbientColour(Ogre::ColourValue colour);
  void setShadowsEnabled(bool enabled);
  void setDebugDrawShadows(bool enabled);
  void setDebugDrawBoundingBoxes(bool enabled);
  void setDebugDrawNavigationMesh(bool enabled);

  //getters
  World* getWorld();
  std::string getName();
  int getSceneID();
  Portal* getPortal(int id = DEFAULT_PORTAL);
  Portal* getDefaultPortal();
  Vector3 getGravity();
  Ogre::SceneManager* getGraphicsManager();
  physx::PxScene* getPhysicsManager();
  physx::PxControllerManager* getControllerManager();
  ArchitectureManager* getArchitectureManager();
  PathfindManager* getPathfindManager();

  //add
  void addPlayer(Player* player, int portalID = DEFAULT_PORTAL);
  void addMonster(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addItem(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addInteractive(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addLight(Vector3 position = Vector3::ZERO, bool castShadows = false, Ogre::Real range = 10, Ogre::ColourValue colour = Ogre::ColourValue());
  void addArchitecture(int id, Vector3 position = Vector3(0,0,0), Quaternion quaternion = Quaternion::IDENTITY, Vector3 scale = Vector3::UNIT_SCALE);
  void addParticles(Ogre::String name, Ogre::String templateName, Vector3 position = Vector3::ZERO, Ogre::Real speed = 1);
  void addPortal(Portal* portal);

  //remove
  void removePlayer(Player* player);
  void removeMonster(Monster* monster);
  void removeItem(Item* item);
  
  //state modification
  void update(double elapsedSeconds);
  void reset();//reverts the scene to its original state
  void build();//generates the navigation mesh and builds static geometry

  //destruction
  void destroyAllAttachedMoveables(Ogre::SceneNode* node);
  void destroySceneNode(Ogre::SceneNode* node);

  //assertions
  bool hasPlayer();//returns true if the local player is within this scene

private:
  World* world;
  SceneDesc desc;
  Portal* defaultEntry;//portal to drop the player at by default. if null pointer then the player will be dropped at zero.

  void setup();
  void release();

  //managers
  Ogre::SceneManager* sceneGraphicsManager;
  physx::PxScene* scenePhysicsManager;
  physx::PxControllerManager* sceneControllerManager;//may only need one manager in the world
  PathfindManager* scenePathfindManager;
  ArchitectureManager* sceneArchitectureManager;

  //scene contents
  Player* localPlayer;//local player
  SceneContainer lights;
  SceneContainer interactives;
  SceneContainer monsters;
  SceneContainer items;
  SceneContainer players;
  std::vector<Portal*> portals;
  std::vector<Ogre::ParticleSystem*> particles;
  Ivy* ivy;//temp ivy test

  //physics
  double accumulator;
  double stepSize;
  physx::PxU32 numberPhysicsCPUThreads;
  bool advancePhysics(double elapsedSeconds);
};

