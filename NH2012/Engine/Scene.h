#pragma once

#include "DataManager.h"
#include "Container.h"
#include "Id.h"
#include "Vector3.h"
#include "Quaternion.h"

//Forward declarations
class World;
class Player;
class Creature;
class Architecture;
class Portal;
class Item;
class Light;
class Interactive;
class Effect;
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
  void reset() {setup();}//reverts the scene to its original state

  //Getters
  World* getWorld() {return world;}
  Id<Scene> getId() {return Id<Scene>(desc.id);}
  std::string getName() {return desc.name;}
  Portal* getDefaultPortal() {return get<Portal>(defaultPortal);}
  
  ScenePhysicsManager* getScenePhysicsManager() {return scenePhysicsManager.get();}
  SceneGraphicsManager* getSceneGraphicsManager() {return sceneGraphicsManager.get();}
  ScenePathfindManager* getScenePathfindManager() {return scenePathfindManager.get();}
  SceneArchitectureManager* getSceneArchitectureManager() {return sceneArchitectureManager.get();}

  //Add
  Id<Item> addItem(int dataId, Vector3 position, Quaternion rotation = Quaternion::IDENTITY, Id<Item> instanceId = Id<Item>());//add an object to the scene
  Id<Effect> addLight(int dataId, Vector3 position, bool castShadows, float range, Id<Effect> instanceId = Id<Effect>());//add an object to the scene
  Id<Portal> addPortal(int dataId, Vector3 position, Vector3 lookAt, Id<Scene> targetScene, Id<Portal> targetPortal, Id<Portal> instanceId = Id<Portal>());//add an object to the scene
  Id<Player> addPlayer(std::shared_ptr<Player> player, Vector3 position, Vector3 lookAt, Id<Player> instanceId = Id<Player>());//add an object to the scene
  Id<Creature> addCreature(int dataId, Vector3 position, Vector3 lookAt = Vector3::ZERO, Id<Creature> instanceId = Id<Creature>());//add an object to the scene
  Id<Interactive> addInteractive(int dataId, Vector3 position, Id<Interactive> instanceId = Id<Interactive>());//add an object to the scene
  Id<Architecture> addArchitecture(int dataId, Vector3 position, Quaternion rotation = Quaternion::IDENTITY, Vector3 scale = Vector3::ZERO, Id<Architecture> instanceId = Id<Architecture>());

  template<class T> T* get(Id<T> id)//get an object from the scene
  {
    Container<T>* container = getContainer<T>();
    if(container) return container->get(id);
    return NULL;
  }

  template<class T> std::shared_ptr<T> remove(Id<T> id)//remove an object from the scene by id
  {
    Container<T>* container = getContainer<T>();
    if(container) return container->remove(id);
    return std::shared_ptr<T>();
  }

  template<class T> std::shared_ptr<T> remove(T* value)//remove an object from the scene by value
  {
    Container<T>* container = getContainer<T>();
    if(container) return container->remove(value);
    return std::shared_ptr<T>();
  }

  template<class T> bool contains(Id<T> id)//returns true if the scene contains the specified object
  {
    Container<T>* container = getContainer<T>();
    if(container) return (container->get(id) != NULL);
    return false;
  }

  template<class T> bool containsAny()//returns true if the scene contains any of the specified object
  {
    Container<T>* container = getContainer<T>();
    if(container) return count() > 0;
    return false;
  }

  template<class T> int count()//counts the specified type of object
  {
    Container<T>* container = getContainer<T>();
    if(container) return container->count();
    return 0;
  }

private:
  SceneDesc desc;
  World* world;

  Id<Player> localPlayer;
  Id<Portal> defaultPortal;

  //Managers
  std::shared_ptr<SceneGraphicsManager> sceneGraphicsManager;
  std::shared_ptr<ScenePhysicsManager> scenePhysicsManager;
  std::shared_ptr<ScenePathfindManager> scenePathfindManager;
  std::shared_ptr<SceneArchitectureManager> sceneArchitectureManager;

  Container<Item> items;
  Container<Player> players;
  Container<Portal> portals;
  Container<Effect> effects;//TODO: add particles to the effects
  Container<Creature> creatures;
  Container<Interactive> interactives;

  void setup();

  void build();//generates the navigation mesh and builds static geometry
  friend class SceneLoader;//we want scene loader to be able to access build()

  template<class T> Container<T>* getContainer()
  {
    if(items.isType<T>())        return (Container<T>*)&items;
    if(players.isType<T>())      return (Container<T>*)&players;
    if(portals.isType<T>())      return (Container<T>*)&portals;
    if(creatures.isType<T>())    return (Container<T>*)&creatures;
    if(effects.isType<T>())      return (Container<T>*)&effects;
    if(interactives.isType<T>()) return (Container<T>*)&interactives;
    return NULL;
  }
};

