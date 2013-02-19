#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "DataManager.h"
#include "ComponentContainer.h"
#include "Flock.h"
#include "Vector3.h"
#include "Quaternion.h"

//#include "PxPhysicsAPI.h"
//#include "characterkinematic/PxControllerManager.h"

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
class Interactive;
class Ivy;
class SceneArchitectureManager;
class SceneGraphicsManager;
class ScenePhysicsManager;
class ScenePathfindManager;


class Scene
{
public:
  Scene(SceneDesc desc, World* world);
  ~Scene(void);

  void update(double elapsedSeconds);
  void reset();//reverts the scene to its original state
  void build();//generates the navigation mesh and builds static geometry

  //Getters
  World* getWorld();
  int getSceneID();
  std::string getName();
  Portal* getPortal(int id = DEFAULT_PORTAL);

  SceneGraphicsManager* getSceneGraphicsManager() {return sceneGraphicsManager;}
  ScenePhysicsManager* getScenePhysicsManager() {return scenePhysicsManager;}
  ScenePathfindManager* getScenePathfindManager() {return scenePathfindManager;}
  SceneArchitectureManager* getSceneArchitectureManager() {return sceneArchitectureManager;}

  //Add
  void addPlayer(Player* player, int portalID = DEFAULT_PORTAL);
  void addMonster(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addItem(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addInteractive(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addLight(Vector3 position = Vector3::ZERO, bool castShadows = false, Ogre::Real range = 10, Ogre::ColourValue colour = Ogre::ColourValue());
  void addArchitecture(int id, Vector3 position = Vector3(0,0,0), Quaternion quaternion = Quaternion::IDENTITY, Vector3 scale = Vector3::UNIT_SCALE);
  void addParticles(std::string name, std::string templateName, Vector3 position = Vector3::ZERO, Ogre::Real speed = 1);
  void addPortal(Portal* portal);

  //Remove
  void removePlayer(Player* player);
  void removeMonster(Monster* monster);
  void removeItem(Item* item);

  //Destruction
  void destroyMonster(Monster* monster);
  void destroyItem(Item* item);

  //Assertions
  bool hasPlayer();//returns true if the local player is within this scene

private:
  World* world;
  SceneDesc desc;
  Portal* defaultEntry;//portal to drop the player at by default. if null pointer then the player will be dropped at zero.

  //Managers
  SceneGraphicsManager* sceneGraphicsManager;
  ScenePhysicsManager* scenePhysicsManager;
  ScenePathfindManager* scenePathfindManager;
  SceneArchitectureManager* sceneArchitectureManager;

  //Contents
  Player* localPlayer;//local player
  ComponentContainer lights;
  ComponentContainer interactives;
  ComponentContainer monsters;
  ComponentContainer items;
  ComponentContainer players;
  std::vector<Portal*> portals;
  std::vector<Ogre::ParticleSystem*> particles;
  Ivy* ivy;//temp ivy test

  void setup();
  void release();
};

