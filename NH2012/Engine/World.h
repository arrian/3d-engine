#pragma once

//Standard
#include <vector>
#include <ctime>
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>

//Boost
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

//OIS
#include <OISKeyboard.h>
#include <OISMouse.h>

//Ogre
#include <OgreFrameListener.h>
#include <OgreVector3.h>
#include <OgreString.h>

//PhysX
#include "PxPhysicsAPI.h"
//#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include "cooking/PxCooking.h"

//Local
#include "DataManager.h"
#include "PhysicsErrorCallback.h"
#include "SceneChangeListener.h"
#include "NHException.h"

//Forward Declarations
class Player;
class Monster;
class Item;
class Scene;



//Keyboard and Mouse Controls
struct Controls
{
  //Move
  OIS::KeyCode moveForward;
  OIS::KeyCode moveLeft;
  OIS::KeyCode moveBack;
  OIS::KeyCode moveRight;

  //Actions
  OIS::KeyCode jump;
  OIS::KeyCode kick;
  OIS::KeyCode run;

  //Hands
  OIS::MouseButtonID leftHand;
  OIS::MouseButtonID rightHand;

  //Miscellaneous
  std::vector<OIS::KeyCode> quickslots;
  OIS::KeyCode exit;

  //Debug
  OIS::KeyCode freezeCollision;
  OIS::KeyCode addItem;
  OIS::KeyCode addMonster;
  OIS::KeyCode reset;
  OIS::KeyCode console;
};

//Entire Game World
class World
{
public:

  ////////////////////////////////////////////////////////////////
  //PhysX Cooking
  class Params
  {
  public:
    Ogre::Vector3 mScale;
    std::map<Ogre::String, physx::PxMaterial*> mMaterialBindings;
    bool mAddBackfaces;

    Params() : mScale(Ogre::Vector3(1,1,1)), mAddBackfaces(false) {}
    ~Params() {}

    Params& scale(const Ogre::Vector3 &scale) { mScale = scale; return *this; }
    Params& materials(std::map<Ogre::String, physx::PxMaterial*> &bindings) { mMaterialBindings = bindings; return *this; }
    Params& backfaces(bool addBackfaces) { mAddBackfaces = addBackfaces; return *this; }
  };

  class AddedMaterials
  {
  public:
    physx::PxMaterial **materials;
    physx::PxU32  materialCount; 

    AddedMaterials() : materials(nullptr) {}
    ~AddedMaterials() { if (materials) delete[] materials; }
  };

  struct MeshInfo
  {
    std::vector<Ogre::Vector3> vertices;//vertex buffer
    std::vector<int> indices;//index buffer
    std::vector<Ogre::String> materials;//assigns a material to each triangle.
  };

  void getMeshInfo(Ogre::MeshPtr mesh, Params &params, MeshInfo &outInfo);
  //void mergeVertices(MeshInfo &outInfo, float fMergeDist = 1e-3f);

  //End PhysX Cooking
  ////////////////////////////////////////////////////////////////////////////////

  World(Ogre::Root* root = 0);
  ~World(void);

  bool frameRenderingQueued(const Ogre::FrameEvent& evt);//perform all world calculations

  //Initialisation
  void initialise(std::string iniFile);
  void parseIni(std::string filename);
  Scene* loadScene(int id);

  //Getters
  DataManager* getDataManager();
  Player* getPlayer();
  Scene* getScene(Ogre::String name);
  Scene* getScene(int id);
  Ogre::Root* getRoot();
  physx::PxPhysics* getPhysics();
  int getNumberScenes();
  void getSceneNames(std::vector<Ogre::String> &names);
  physx::PxMaterial* getDefaultPhysicsMaterial();
  SceneChangeListener* getSceneChangeListener();
  const physx::PxTolerancesScale& getTolerancesScale();//messy tolerance bind

  //Creation
  Item* createItem(int id);
  Monster* createMonster(int id);
  //Mesh physics creation... extract to appropriate class
  physx::PxConvexMesh* createConvexMesh(Ogre::Entity* e);
  physx::PxTriangleMesh* createTriangleMesh(Ogre::Entity* e);
  physx::PxTriangleMesh* createTriangleMeshV2(Ogre::Entity* e, Params &params = Params(), AddedMaterials *out_addedMaterials = nullptr);

  //Removal
  void releaseItem(Item* item);
  void releaseMonster(Monster* monster);
  bool destroyScene(Ogre::String name);

  //Setters
  void setSceneChangeListener(SceneChangeListener* listener);
  void setSceneManager(Ogre::SceneManager* sceneManager);
  void hookWindow(Ogre::RenderWindow* window);//convenience method for hooking the render window to the player
  void movePlayer(Player* player, Scene* target);/* Moves the player to the target scene.*/
  
  //Injection
  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  //Assertions
  bool hasScene(int id);
  bool isDebug();

  //Flags... extract to settings class
  bool debug;
  bool verbose;
  bool enablePhysics;
  bool enableAI;
  bool enableAudio;
  bool enableHDR;
  bool enableBloom;
  bool enableShadows;
  bool enableLights;
  bool enableParticles;
  bool enableDecals;
  bool enableSprites;
  bool enableWater;
  bool enableSky;
  bool freeCameraDebug;
  bool wireframeDebug;
  bool freezeCollisionDebug;
  bool showCollisionDebug;
  bool showShadowDebug;
  float gravity;
  Ogre::String sceneDataFilename;
  Ogre::String architectureDataFilename;
  Ogre::String monsterDataFilename;
  Ogre::String itemDataFilename;
  Ogre::String soundDataFilename;
  Controls controls;/*! Stores the control mapping.*/

private:

  Ogre::Root* root;
  SceneChangeListener* sceneChangeListener;
  DataManager dataManager;
  
  //World Contents
  Player* player;//std::vector<Player> players;
  std::vector<Monster*> monsters;
  std::vector<Item*> items;
  std::vector<Scene*> scenes;

  //PhysX
  PhysicsErrorCallback errorCallback;
  physx::PxDefaultAllocator allocatorCallback;//for allocating memory to physics manager
  physx::PxFoundation* physicsFoundation;
  physx::PxCooking* physicsCooking;
  physx::PxPhysics* physicsWorld;
  physx::PxMaterial* physicsMaterial;//default material

  //Defaults
  physx::PxReal defaultStaticFriction;
  physx::PxReal defaultDynamicFriction;
  physx::PxReal defaultRestitution;

};

