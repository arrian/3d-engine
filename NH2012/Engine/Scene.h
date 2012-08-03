#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "SceneType.h"
#include "World.h"
#include "Player.h."
#include "Monster.h"
#include "Item.h"
#include "Architecture.h"
#include "Environment.h"

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"

//forward declaring for circular dependency
class World;
class Player;
class Monster;

class Scene
{
public:
  Scene(World* world, Ogre::String name = Ogre::String("Default Dungeon"), 
       SceneType type = PREDEFINED);
  ~Scene(void);

  Ogre::String getName();

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

  int getSceneID();

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  bool isActive();

  int getNewInstanceNumber();

private:
  World* world;
  Ogre::SceneManager* sceneManager;
  physx::PxScene* physicsManager;
  physx::PxControllerManager* controllerManager;//may only need one manager in the world

  Ogre::String name;
  SceneType type;

  Architecture* architecture;
  std::vector<Ogre::Light*> lights;
  std::vector<Ogre::ParticleSystem*> particles;
  std::vector<Monster*> monsters;
  std::vector<Item*> items;
  Player* player;

  bool active;

  int instanceNumber;
  
  /*void generatePredefined();*/
  void load(std::string String file);

  bool advancePhysics(Ogre::Real dt);
};

