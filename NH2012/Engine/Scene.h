#pragma once

#include <boost/any.hpp>

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "DataManager.h"
#include "Container.h"
#include "Flock.h"
#include "Vector3.h"
#include "Quaternion.h"

#include "Id.h"

//#include "PxPhysicsAPI.h"
//#include "characterkinematic/PxControllerManager.h"

//statics
static const int DEFAULT_PORTAL = -1;

//forward declarations
class World;
class Player;
class Creature;
class Portal;
class Item;
class Light;
class Flock;
class BoidExtension;
class Interactive;
class Effect;
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

  //Getters
  World* getWorld() {return world;}
  int getId() {return desc.id;}
  Id<Scene> getInstanceId() {return id;}
  std::string getName() {return desc.name;}
  
  SceneGraphicsManager* getSceneGraphicsManager() {return sceneGraphicsManager;}
  ScenePhysicsManager* getScenePhysicsManager() {return scenePhysicsManager;}
  ScenePathfindManager* getScenePathfindManager() {return scenePathfindManager;}
  SceneArchitectureManager* getSceneArchitectureManager() {return sceneArchitectureManager;}

  template<class T> Id<T> add(std::shared_ptr<T> t)//add an object to the scene
  {
    Container<T>* container = getContainer<T>();
    if(!container) return Id<T>(-1);
    Id<T> id = Id<T>();
    container->insert(id, t);
    return id;
  }

  template<class T> std::shared_ptr<T> get(Id<T> id)//get an object from the scene
  {
    Container<T>* container = getContainer<T>();
    if(!container) return NULL;
    container->get(id);
  }

  template<class T> void remove(Id<T> id)//remove an object from the scene
  {
    Container<T>* container = getContainer<T>();
    if(!container) return;
    container->remove(id);
  }

  template<class T> bool contains(Id<T> id)//returns true if the scene contains the specified object
  {
    Container<T>* container = getContainer<T>();
    if(!container) return false;
    container->get(id);
  }

  template<class T> int count()//counts the specified type of object
  {
    Container<T>* container = getContainer<T>();
    if(!container) return 0;
    container->count();
  }

  ////////////////////////////////////////////deprecated below

  /*
  //Add - deprecated
  void addPortal(Portal* portal);
  void addPlayer(Player* player, int portalID = DEFAULT_PORTAL);
  void addCreature(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addItem(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addInteractive(int id, Vector3 position = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY);
  void addLight(Vector3 position = Vector3::ZERO, bool castShadows = false, Ogre::Real range = 10, Ogre::ColourValue colour = Ogre::ColourValue());
  void addArchitecture(int id, Vector3 position = Vector3(0,0,0), Quaternion quaternion = Quaternion::IDENTITY, Vector3 scale = Vector3::UNIT_SCALE);
  void addParticles(std::string name, std::string templateName, Vector3 position = Vector3::ZERO, Ogre::Real speed = 1);
  
  //Getters - deprecated
  Portal* getPortal(int id = DEFAULT_PORTAL);

  //Remove - deprecated
  void removePlayer(Player* player);
  void removeCreature(Creature* creature);
  void removeItem(Item* item);

  //Destruction - deprecated
  void destroyCreature(Creature* creature);
  void destroyItem(Item* item);

  //Assertions
  bool hasPlayer();//returns true if the local player is within this scene
  */
private:
  Id<Scene> id;
  SceneDesc desc;
  World* world;
  Portal* defaultEntry;//portal to drop the player at by default. if null pointer then the player will be dropped at zero.

  //Managers
  SceneGraphicsManager* sceneGraphicsManager;
  ScenePhysicsManager* scenePhysicsManager;
  ScenePathfindManager* scenePathfindManager;
  SceneArchitectureManager* sceneArchitectureManager;

  void setup();
  void release();

  void build();//generates the navigation mesh and builds static geometry
  friend class SceneLoader;//we want scene loader to be able to access build()

  //The scene contents using instance as the key
  Container<Item> items;
  Container<Light> lights;
  Container<Player> players;
  Container<Portal> portals;
  Container<Effect> effects;//TODO: add particles to the effects
  Container<Creature> creatures;
  Container<Interactive> interactives;

  template<class T> Container<T>* getContainer()
  {
    if(items.isType<T>())        return &items;
    if(lights.isType<T>())       return &lights;
    if(players.isType<T>())      return &players;
    if(portals.isType<T>())      return &portals;
    if(creatures.isType<T>())    return &creatures;
    if(effects.isType<T>())      return &effects;
    if(interactives.isType<T>()) return &interactives;
    return NULL;
  }
  
  //Contents - deprecated
  /*
  Player* localPlayer;//local player
  ComponentContainer lights;
  ComponentContainer interactives;
  ComponentContainer creatures;
  ComponentContainer items;
  ComponentContainer players;
  std::vector<Portal*> portals;
  std::vector<Ogre::ParticleSystem*> particles;
  Ivy* ivy;//temp ivy test
  */

};

