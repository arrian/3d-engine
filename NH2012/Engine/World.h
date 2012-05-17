#pragma once

#include <vector>

#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreFrameListener.h>

#include "PxPhysicsAPI.h"
//#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include "cooking/PxCooking.h"

#include "SceneType.h"
#include "Environment.h"
#include "DataManager.h"
#include "PhysicsErrorCallback.h"
#include "SceneChangeListener.h"
#include "NHException.h"

class Player;
class Monster;
class Item;
class Scene;

class World : public Environment
{
public:
  World(Ogre::Root* root);
  ~World(void);

  void initialise(std::string iniFile);

  Player* getPlayer();
  Scene* getScene(Ogre::String name);
  Scene* getScene(unsigned int index);

  Ogre::Root* getRoot();
  physx::PxPhysics* getPhysics();
  const physx::PxTolerancesScale& getTolerancesScale();//messy tolerance bind

  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player

  /* Moves the player to the target scene.*/
  void movePlayer(Player* player, Scene* target);

  int getNumberScenes();
  void getSceneNames(std::vector<Ogre::String> &names);

  /* Loads a scene. Returns the loaded scene.*/
  Scene* loadScene(Ogre::String name, SceneType type);

  Item* createItem();
  Monster* createMonster();
  physx::PxConvexMesh* createConvexMesh(Ogre::Entity* e);
  physx::PxTriangleMesh* createTriangleMesh(Ogre::Entity* e);
  void releaseItem(Item* item);
  void releaseMonster(Monster* monster);

  /* Destroys a scene. Returns true if success.*/
  bool destroyScene(Ogre::String name);

  SceneChangeListener* getSceneChangeListener();
  void setSceneChangeListener(SceneChangeListener* listener);

  void setSceneManager(Ogre::SceneManager* sceneManager);
  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations

  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  physx::PxMaterial* getDefaultPhysicsMaterial();
private:
  Ogre::Root* root;
  SceneChangeListener* sceneChangeListener;
  
  //PhysX
  PhysicsErrorCallback errorCallback;
  physx::PxDefaultAllocator allocatorCallback;//for allocating memory to physics manager
  physx::PxFoundation* physicsFoundation;
  physx::PxCooking* physicsCooking;
  physx::PxPhysics* physicsWorld;
  physx::PxMaterial* physicsMaterial;//default material

  //World Items
  Player* player;//std::vector<Player> players;
  std::vector<Monster*> monsters;
  std::vector<Item*> items;
  std::vector<Scene*> scenes;
};

