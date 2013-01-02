#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "DataManager.h"
#include "PathfindManager.h"
#include "Flock.h"
#include "Vector3.h"
#include "Quaternion.h"

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"

//need to include this directory
#include "../../RapidXml/rapidxml-1.13/rapidxml.hpp"

static const int DEFAULT_PORTAL = -1;

//forward declaring for circular dependency
class World;
class Player;
class Monster;
class Portal;
class Flock;
class BoidExtension;
class Item;
class Light;
class Architecture;
class Interactive;

class Scene
{
public:
  Scene(SceneDesc desc, World* world);
  ~Scene(void);

  void setGravity(Vector3 gravity);
  void setShadowsEnabled(bool enabled);
  void setDebugDrawShadows(bool enabled);
  void setDebugDrawBoundingBoxes(bool enabled);
  void setDebugDrawNavigationMesh(bool enabled);

  Ogre::String getName();
  int getSceneID();
  Portal* getPortal(int id = DEFAULT_PORTAL);
  Portal* getDefaultPortal();
  Vector3 getGravity();

  Ogre::SceneManager* getSceneManager();
  physx::PxScene* getPhysicsManager();
  physx::PxControllerManager* getControllerManager();
  World* getWorld();
  Architecture* getArchitecture();
  PathfindManager* getPathfindManager();

  void addPlayer(Player* player, int portalID = DEFAULT_PORTAL);
  void addMonster(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addItem(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addInteractive(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addLight(Vector3 position = Vector3::ZERO, bool castShadows = false, Ogre::Real range = 10, Ogre::ColourValue colour = Ogre::ColourValue());
  void addParticles(Ogre::String name, Ogre::String templateName, Vector3 position = Vector3::ZERO, Ogre::Real speed = 1);
  void addPortal(Portal* portal);

  void removePlayer(Player* player);
  void removeMonster(Monster* monster);
  void removeItem(Item* item);
  
  void update(double elapsedSeconds);

  //utility to detroy a scene node and all child nodes and entities
  void destroyAllAttachedMoveables(Ogre::SceneNode* node);
  void destroySceneNode(Ogre::SceneNode* node);

  void reset();

  bool hasPlayer();
  
private:
  SceneDesc desc;
  Portal* defaultEntry;//portal to drop the player at by default. if null pointer then the player will be dropped at zero.
  
  Ogre::ColourValue defaultAmbientColour;

  World* world;
  Ogre::SceneManager* sceneManager;
  physx::PxScene* physicsManager;
  physx::PxControllerManager* controllerManager;//may only need one manager in the world
  PathfindManager* pathfinder;
  
  Architecture* architecture;
  std::vector<Light*> lights;
  std::vector<Ogre::ParticleSystem*> particles;
  std::vector<Monster*> monsters;
  std::vector<Item*> items;
  std::vector<Interactive*> interactives;
  std::vector<Portal*> portals;
  Player* player;

  void load(std::string file);
  Vector3 getXMLVector(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third);
  Quaternion getXMLRotation(rapidxml::xml_node<>* node);
  Ogre::ColourValue getXMLColour(rapidxml::xml_node<>* node, std::string first = "cr", std::string second = "cg", std::string third = "cb", std::string fourth = "ca");
  Vector3 getXMLScale(rapidxml::xml_node<>* node);
  Vector3 getXMLPosition(rapidxml::xml_node<>* node);
  
  bool advancePhysics(double elapsedSeconds);
  double accumulator;
  double stepSize;

  physx::PxU32 numberPhysicsCPUThreads;

  void setup();
  void release();
  
};

