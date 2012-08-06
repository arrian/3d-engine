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
#include "Environment.h"

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"

//need to include this directory
#include "../../RapidXml/rapidxml-1.13/rapidxml.hpp"

//forward declaring for circular dependency
class World;
class Player;
class Monster;

class Scene
{
public:
  Scene(World* world, int id = 0);
  ~Scene(void);

  Ogre::String getName();
  int getSceneID();
  int getNewInstanceNumber();

  void addPlayer(Player* player);
  void addMonster(int id, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY);
  void addItem(int id, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY);
  void addLight(Ogre::Vector3 position = Ogre::Vector3::ZERO, bool castShadows = false, Ogre::Real range = 10, Ogre::ColourValue colour = Ogre::ColourValue());
  void addParticles(Ogre::String name, Ogre::Vector3 position = Ogre::Vector3::ZERO, Ogre::Vector3 scale = Ogre::Vector3::UNIT_SCALE, Ogre::Real speed = 1);

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
  
  
  World* world;
  Ogre::SceneManager* sceneManager;
  physx::PxScene* physicsManager;
  physx::PxControllerManager* controllerManager;//may only need one manager in the world

  Architecture* architecture;
  std::vector<Ogre::Light*> lights;
  std::vector<Ogre::ParticleSystem*> particles;
  std::vector<Monster*> monsters;
  std::vector<Item*> items;
  Player* player;

  /*Defines if the scene should receive frame updates.*/
  bool active;

  /*Tracks the identification numbers for the objects in the scene.*/
  int instanceNumber;
  
  /*void generatePredefined();*/
  void load(std::string file);
  Ogre::Vector3 getXMLPosition(rapidxml::xml_node<>* node, std::string first = "tx", std::string second = "ty", std::string third = "tz");
  Ogre::Quaternion getXMLRotation(rapidxml::xml_node<>* node);
  Ogre::ColourValue getXMLColour(rapidxml::xml_node<>* node);
  Ogre::Vector3 getXMLScale(rapidxml::xml_node<>* node);
  
  bool advancePhysics(Ogre::Real dt);
  float accumulator;
  float stepSize;
};

