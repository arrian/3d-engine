#pragma once

//Standard
#include <map>
#include <string>

//Local
#include "DataManager.h"
#include "TimeManager.h"
#include "SoundManager.h"
#include "ScriptManager.h"
#include "ControlManager.h"
#include "PhysicsManager.h"
#include "NetworkManager.h"
#include "GraphicsManager.h"

#include "Id.h"
#include "Container.h"

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

//Forward Declarations
class Player;
class Scene;
class WorldLoader;

/************************************************************************/
/* Contains the entire game world.                                      */
/************************************************************************/
class World : public boost::enable_shared_from_this<World>
{
public:
  World();
  World(boost::shared_ptr<Ogre::Root> root, Ogre::RenderWindow* window);
  ~World(void);

  typedef boost::shared_ptr<World> SharedPointer;
  typedef boost::weak_ptr<World> WeakPointer;

  bool update(double elapsedSeconds);

  //Initialisation
  void initialise(std::string iniFile);
  boost::shared_ptr<Scene> loadScene(Id<Scene> id);
  boost::shared_ptr<Player> loadPlayer(Id<Player> id);

  //Destruction
  void destroyScene(Id<Scene> id);

  //Getters
  DataManager* getDataManager();
  TimeManager* getTimeManager();
  SoundManager* getSoundManager();
  ScriptManager* getScriptManager();
  PhysicsManager* getPhysicsManager();
  ControlManager* getControlManager();
  NetworkManager* getNetworkManager();
  GraphicsManager* getGraphicsManager();
  boost::shared_ptr<Player> getPlayer();
  //Scene* getScene(Id<Scene> id);
  boost::shared_ptr<Scene> getScene(Id<Scene> id);
  int getSceneCount();
  void getSceneNames(std::map<Id<Scene>, std::string> &names);

  //Input
  void keyPressed(const OIS::KeyEvent &arg);
  void keyReleased(const OIS::KeyEvent &arg);
  void mouseMoved(const OIS::MouseEvent &arg);
  void mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

  //Assertions
  bool hasScene(Id<Scene> id);

  void setPlayerScene(Id<Scene> sceneId, Vector3 position, Vector3 lookAt);

private:
  //Local Player
  Id<Player> playerId;
  boost::shared_ptr<Player> player;

  //Scenes
  Id<Scene> defaultScene;
  //Container<Scene> scenes;
  boost::shared_ptr<Scene> scene;
  
  //Managers
  DataManager dataManager;
  TimeManager timeManager;
  SoundManager soundManager;
  ScriptManager scriptManager;
  PhysicsManager physicsManager;
  ControlManager controlManager;
  NetworkManager networkManager;
  GraphicsManager graphicsManager;
  
  //Initialisation
  friend class WorldLoader;
};

