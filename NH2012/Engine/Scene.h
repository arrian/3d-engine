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
  void addMonster(Ogre::Vector3 position);
  void addItem(Ogre::Vector3 position);
  void addLight(Ogre::Vector3 position, bool castShadows, Ogre::Real range);
  void addParticles(Ogre::String name, Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::Real speed);

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
  Ogre::Vector3 getXMLPosition(rapidxml::xml_node<>* node);
  Ogre::Quaternion getXMLRotation(rapidxml::xml_node<>* node);
  Ogre::ColourValue getXMLColour(rapidxml::xml_node<>* node);
  

  bool advancePhysics(Ogre::Real dt);
  float accumulator;
  float stepSize;
};

