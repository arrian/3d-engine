#pragma once

//Boost
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

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


class Scene : public boost::enable_shared_from_this<Scene>
{
public:
  Scene(SceneDesc desc, boost::shared_ptr<World> world);
  ~Scene(void);

  typedef boost::shared_ptr<Scene> SharedPointer;
  typedef boost::weak_ptr<Scene> WeakPointer;

  void update(double elapsedSeconds);

  void initialise();
  void reset();//reverts the scene to its original state

  //Getters
  boost::shared_ptr<World> getWorld() 
  {
    try
    {
      return world.lock();
    }
    catch(boost::bad_weak_ptr b)
    {
#ifdef _DEBUG
      std::cout << "Could not get world from scene. World has expired." << std::endl;
#endif
    }
    return boost::shared_ptr<World>();
  }

  Id<Scene> getId() {return Id<Scene>(desc.id);}
  std::string getName() {return desc.name;}
  boost::shared_ptr<Portal> getDefaultPortal() {return get<Portal>(defaultPortal);}
  
  boost::shared_ptr<ScenePhysicsManager> getScenePhysicsManager() {return scenePhysicsManager;}
  boost::shared_ptr<SceneGraphicsManager> getSceneGraphicsManager() {return sceneGraphicsManager;}
  boost::shared_ptr<ScenePathfindManager> getScenePathfindManager() {return scenePathfindManager;}
  boost::shared_ptr<SceneArchitectureManager> getSceneArchitectureManager() {return sceneArchitectureManager;}

  //Add
  Id<Item> addItem(int dataId, Vector3 position, Quaternion rotation = Quaternion::IDENTITY, Id<Item> instanceId = Id<Item>());//add an object to the scene
  Id<Effect> addLight(int dataId, Vector3 position, bool castShadows, float range, Id<Effect> instanceId = Id<Effect>());//add an object to the scene
  Id<Portal> addPortal(int dataId, Vector3 position, Vector3 lookAt, Id<Scene> targetScene, Id<Portal> targetPortal, Id<Portal> instanceId = Id<Portal>());//add an object to the scene
  Id<Player> addPlayer(boost::shared_ptr<Player> player, Vector3 position, Vector3 lookAt, Id<Player> instanceId = Id<Player>());//add an object to the scene
  Id<Creature> addCreature(int dataId, Vector3 position, Vector3 lookAt = Vector3::ZERO, Id<Creature> instanceId = Id<Creature>());//add an object to the scene
  Id<Interactive> addInteractive(int dataId, Vector3 position, Id<Interactive> instanceId = Id<Interactive>());//add an object to the scene
  Id<Architecture> addArchitecture(int dataId, Vector3 position, Quaternion rotation = Quaternion::IDENTITY, Vector3 scale = Vector3::ZERO, Id<Architecture> instanceId = Id<Architecture>());

  template<class T> boost::shared_ptr<T> get(Id<T> id)//get an object from the scene
  {
    Container<T>* container = getContainer<T>();
    if(container) return container->get(id);
    return boost::shared_ptr<T>();
  }

  template<class T> boost::shared_ptr<T> remove(Id<T> id)//remove an object from the scene by id
  {
    Container<T>* container = getContainer<T>();
    if(container) return container->remove(id);
    return boost::shared_ptr<T>();
  }

  template<class T> boost::shared_ptr<T> remove(T* value)//remove an object from the scene by value
  {
    Container<T>* container = getContainer<T>();
    if(container) return container->remove(value);
    return boost::shared_ptr<T>();
  }

  template<class T> bool contains(Id<T> id)//returns true if the scene contains the specified object
  {
    Container<T>* container = getContainer<T>();
    if(container) return container->get(id);
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
  boost::weak_ptr<World> world;

  Id<Player> localPlayer;
  Id<Portal> defaultPortal;

  //Managers
  boost::shared_ptr<SceneGraphicsManager> sceneGraphicsManager;
  boost::shared_ptr<ScenePhysicsManager> scenePhysicsManager;
  boost::shared_ptr<ScenePathfindManager> scenePathfindManager;
  boost::shared_ptr<SceneArchitectureManager> sceneArchitectureManager;

  Container<Item> items;
  Container<Player> players;
  Container<Portal> portals;
  Container<Effect> effects;//TODO: add particles to the effects
  Container<Creature> creatures;
  Container<Interactive> interactives;

  

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

