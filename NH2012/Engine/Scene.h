#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "World.h"
#include "Player.h."
#include "Monster.h"
#include "Item.h"
#include "Architecture.h"
#include "Portal.h"

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"

//need to include this directory
#include "../../RapidXml/rapidxml-1.13/rapidxml.hpp"

//forward declaring for circular dependency
class World;
class Player;
class Monster;
class Portal;

class Scene
{
public:
  Scene(World* world, int id = 0);
  ~Scene(void);

  Ogre::String getName();
  int getSceneID();
  int getNewInstanceNumber();
  Portal* getPortal(int id = -1);
  Portal* getDefaultPortal();

  void addPlayer(Player* player, int portalID = -1);
  void addMonster(int id, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY);
  void addItem(int id, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY);
  void addLight(Ogre::Vector3 position = Ogre::Vector3::ZERO, bool castShadows = false, Ogre::Real range = 10, Ogre::ColourValue colour = Ogre::ColourValue());
  void addParticles(Ogre::String name, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Real speed = 1);
  void addPortal(Portal* portal);

  void removePlayer(Player* player);
  
  Ogre::SceneManager* getSceneManager();
  physx::PxScene* getPhysicsManager();
  physx::PxControllerManager* getControllerManager();
  World* getWorld();
  
  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  bool isActive();
  
private:
  int id;
  Ogre::String name;
  float north;
  Portal* defaultEntry;//portal to drop the player at by default. if null pointer then the player will be dropped at zero.
  
  Ogre::ColourValue defaultAmbientColour;

  World* world;
  Ogre::SceneManager* sceneManager;
  physx::PxScene* physicsManager;
  physx::PxControllerManager* controllerManager;//may only need one manager in the world

  Architecture* architecture;
  std::vector<Ogre::Light*> lights;
  std::vector<Ogre::ParticleSystem*> particles;
  std::vector<Monster*> monsters;
  std::vector<Item*> items;
  std::vector<Portal*> portals;
  Player* player;

  /*Defines if the scene should receive frame updates.*/
  bool active;

  /*Tracks the identification numbers for the objects in the scene.*/
  int instanceNumber;
  
  /*void generatePredefined();*/
  void load(std::string file);
  Ogre::Vector3 getXMLVector(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third);
  Ogre::Quaternion getXMLRotation(rapidxml::xml_node<>* node);
  Ogre::ColourValue getXMLColour(rapidxml::xml_node<>* node, std::string first = "cr", std::string second = "cg", std::string third = "cb", std::string fourth = "ca");
  Ogre::Vector3 getXMLScale(rapidxml::xml_node<>* node);
  Ogre::Vector3 getXMLPosition(rapidxml::xml_node<>* node);
  
  bool advancePhysics(Ogre::Real dt);
  float accumulator;
  float stepSize;
};

