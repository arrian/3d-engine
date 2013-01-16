#include "Scene.h"

#include <algorithm>

#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSimpleFactory.h"
#include "PxVisualizationParameter.h"

#include "World.h"
#include "Player.h."
#include "Monster.h"
#include "Item.h"
#include "Light.h"
#include "Portal.h"
#include "Flock.h"
#include "Door.h"
#include "Chest.h"
#include "Ivy.h"

#include "SceneLoader.h"
#include "ArchitectureManager.h"

//-------------------------------------------------------------------------------------
Scene::Scene(SceneDesc desc, World* world)
  : world(world),
    desc(desc),
    sceneGraphicsManager(NULL),
    sceneControllerManager(NULL),
    scenePhysicsManager(NULL),
    defaultEntry(NULL),
    localPlayer(NULL),
    numberPhysicsCPUThreads(4),
    stepSize(1.0 / 60.0),
    accumulator(0.0),
    scenePathfindManager(NULL),
    particles(),
    monsters(),
    portals(),
    lights(),
    items(),
    players()
{
  setup();
  
  ivy = new Ivy(this, Vector3(0,-2,5), Vector3(0,-1,0));
  addInteractive(0);
}

//-------------------------------------------------------------------------------------
Scene::~Scene(void)
{
  delete ivy;
  release();
}

//-------------------------------------------------------------------------------------
Ogre::String Scene::getName()
{
  return desc.name;
}

//-------------------------------------------------------------------------------------
Ogre::SceneManager* Scene::getGraphicsManager()
{
  return sceneGraphicsManager;
}

//-------------------------------------------------------------------------------------
physx::PxScene* Scene::getPhysicsManager()
{
  return scenePhysicsManager;
}

//-------------------------------------------------------------------------------------
physx::PxControllerManager* Scene::getControllerManager()
{
  return sceneControllerManager;
}

//-------------------------------------------------------------------------------------
void Scene::addPlayer(Player* player, int portalID)
{
  Vector3 position;
  Vector3 lookAt;
  if(defaultEntry != NULL && portalID == DEFAULT_PORTAL)//use default portal
  {
    position = defaultEntry->getPosition();
    lookAt = defaultEntry->getLookAt();
  }
  else if(portalID >= 0)//use specified portal
  {
    Portal* portal = getPortal(portalID);
    position = portal->getPosition();
    lookAt = portal->getLookAt();
  }
  else//place the player at zero
  {
    position = Vector3::ZERO;
    lookAt = Vector3::UNIT_Z;
  }

  Scene* old = player->getScene();
  if(old) old->removePlayer(player);//need to remove player from previous scene
  player->setScene(this);
  player->setPosition(position);
  player->setLookAt(lookAt);
  this->localPlayer = player;
}

//-------------------------------------------------------------------------------------
void Scene::addMonster(int id, Vector3 position, Quaternion rotation)
{
  Monster* monster = new Monster(world->getDataManager()->getMonster(id));
  monster->setScene(this);
  monster->setPosition(position);
  monster->setGoal(new Go(getPathfindManager()->getRandomNavigablePoint(), Priority::HIGH));
  monsters.add(monster);
}

//-------------------------------------------------------------------------------------
void Scene::addItem(int id, Vector3 position, Quaternion rotation)
{
  Item* item = new Item(world->getDataManager()->getItem(id));//this->getWorld()->createItem(id);
  item->setScene(this);
  item->setPosition(position);
  item->setRotation(rotation);
  items.add(item);
}

//-------------------------------------------------------------------------------------
void Scene::addInteractive(int id, Vector3 position, Quaternion rotation)
{
  Interactive* interactive = NULL;
  if(id == 0) interactive = new Door();
  else if(id == 1) interactive = new Chest();
  else throw NHException("the specified interactive object id has not been implemented");

  interactive->setScene(this);
  interactive->setPosition(position);
  interactive->setRotation(rotation);
  interactives.add(interactive);
}

//-------------------------------------------------------------------------------------
void Scene::addLight(Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour)
{
  Light* light = new Light(this, position, castShadows, range, colour);
  lights.add(light);
}

//-------------------------------------------------------------------------------------
void Scene::addArchitecture(int id, Vector3 position, Quaternion quaternion, Vector3 scale)
{
  sceneArchitectureManager->add(world->getDataManager()->getArchitecture(id), position, quaternion, scale);
}

//-------------------------------------------------------------------------------------
void Scene::addParticles(Ogre::String name, Ogre::String templateName, Vector3 position, Ogre::Real speed)
{
  Ogre::ParticleSystem* particle = sceneGraphicsManager->createParticleSystem(name, templateName);//"Rain");//, "Examples/Rain");
  particle->setSpeedFactor(speed);
  Ogre::SceneNode* particleNode = sceneGraphicsManager->getRootSceneNode()->createChildSceneNode();//"particle" + Ogre::StringConverter::toString(getNewInstanceNumber()));
  particleNode->setPosition(position);
  particleNode->attachObject(particle);
  particles.push_back(particle);
}

//-------------------------------------------------------------------------------------
void Scene::addPortal(Portal* portal)
{
  portals.push_back(portal);
  if(defaultEntry == NULL) defaultEntry = portals[0];//creating a default entry point
}

//-------------------------------------------------------------------------------------
void Scene::removePlayer(Player* player)
{
  if(this->localPlayer == player) this->localPlayer = NULL;
}

//-------------------------------------------------------------------------------------
void Scene::removeItem(Item* item)
{
  items.destroy(item);
}

//-------------------------------------------------------------------------------------
void Scene::removeMonster(Monster* monster)
{
  monsters.destroy(monster);
}

//-------------------------------------------------------------------------------------
void Scene::update(double elapsedSeconds)
{

  //ivy->update(elapsedSeconds);
  
  scenePathfindManager->update(elapsedSeconds);//for nav mesh updates

  lights.update(elapsedSeconds);
  monsters.update(elapsedSeconds);
  items.update(elapsedSeconds);
  interactives.update(elapsedSeconds);
  players.update(elapsedSeconds);

  if(localPlayer) localPlayer->update(elapsedSeconds);
  
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it) 
  {
    if((*it)->isLoadRequired(localPlayer->getPosition())) world->loadScene((*it)->getID());
  }

  //pre simulation updates

  if(world->isPhysicsEnabled()) advancePhysics(elapsedSeconds);

  //post simulation events
}

//-------------------------------------------------------------------------------------
bool Scene::advancePhysics(double elapsedSeconds)
{
  accumulator += elapsedSeconds;
  if(accumulator < stepSize) return false;
  accumulator -= stepSize;
  scenePhysicsManager->simulate((float) stepSize);
  scenePhysicsManager->fetchResults(true);

  return true;
}

/*
//XML scene file string constants
static const char SCENE_STRING[] = "scene";
static const char AMBIENT_RED_STRING[] = "ambient_r";
static const char AMBIENT_GREEN_STRING[] = "ambient_g";
static const char AMBIENT_BLUE_STRING[] = "ambient_b";
static const char NORTH_STRING[] = "north";
static const char ARCHITECTURE_STRING[] = "architecture";
static const char ID_STRING[] = "id";
static const char LIGHT_STRING[] = "light";
static const char CAST_SHADOWS_STRING[] = "cast_shadows";
static const char RANGE_STRING[] = "range";
static const char ITEM_STRING[] = "item";
static const char MONSTER_STRING[] = "monster";
static const char PORTAL_STRING[] = "portal";
static const char TARGET_SCENE_ID_STRING[] = "target_scene_id";
static const char TARGET_PORTAL_ID_STRING[] = "target_portal_id";
static const char LOOK_AT_X_STRING[] = "ltx";
static const char LOOK_AT_Y_STRING[] = "lty";
static const char LOOK_AT_Z_STRING[] = "ltz";
static const char PARTICLE_STRING[] = "particle";
static const char NAME_STRING[] = "name";
static const char TEMPLATE_NAME_STRING[] = "template_name";

static const char TRANSLATION_X_STRING[] = "tx";
static const char TRANSLATION_Y_STRING[] = "ty";
static const char TRANSLATION_Z_STRING[] = "tz";
static const char ROTATION_X_STRING[] = "rx";
static const char ROTATION_Y_STRING[] = "ry";
static const char ROTATION_Z_STRING[] = "rz";
static const char SCALE_X_STRING[] = "sx";
static const char SCALE_Y_STRING[] = "sy";
static const char SCALE_Z_STRING[] = "sz";

//-------------------------------------------------------------------------------------
void Scene::load(std::string file)
{
  try
  {
    Ogre::FileInfoListPtr files = Ogre::ResourceGroupManager::getSingletonPtr()->findResourceFileInfo("Essential", file);
    if(files->size() < 1) throw NHException("could not find the path to the specified scene");

    std::string filePath = files->front().archive->getName() + "/" + files->front().filename;

    std::ifstream streamfile(filePath);
    rapidxml::xml_document<> doc;

    std::vector<char> buffer((std::istreambuf_iterator<char>(streamfile)), std::istreambuf_iterator<char>());//streamfile)), std::istreambuf_iterator<char>());

    buffer.push_back('\0');//null terminating the buffer

    //std::cout << &buffer[0] << std::endl; //test the buffer

    doc.parse<0>(&buffer[0]);
    rapidxml::xml_node<>* root = doc.first_node(SCENE_STRING);//"scene");

    defaultAmbientColour = getXMLColour(root, AMBIENT_RED_STRING, AMBIENT_GREEN_STRING, AMBIENT_BLUE_STRING);//"ambient_r", "ambient_g", "ambient_b");

    
    //description attributes
    //north = boost::lexical_cast<float>(root->first_attribute(NORTH_STRING)->value());
   
    //Architecture
    rapidxml::xml_node<>* architectureNode = root->first_node(ARCHITECTURE_STRING);//"architecture");
    while(architectureNode != NULL)
    {
      int id = boost::lexical_cast<int>(architectureNode->first_attribute(ID_STRING)->value());
      sceneArchitectureManager->add(world->getDataManager()->getArchitecture(id), getXMLPosition(architectureNode), getXMLRotation(architectureNode), getXMLScale(architectureNode));
      architectureNode = architectureNode->next_sibling(ARCHITECTURE_STRING);
    }

    build();//building static geometry - must do here else we can't use the navigation mesh for monster placement
   
    //Lights
    rapidxml::xml_node<>* lightNode = root->first_node(LIGHT_STRING);//"light");
    while(lightNode != NULL)
    {
      bool cast_shadows = boost::lexical_cast<bool>(lightNode->first_attribute(CAST_SHADOWS_STRING)->value());
      float range = boost::lexical_cast<float>(lightNode->first_attribute(RANGE_STRING)->value());
      Ogre::ColourValue colour = getXMLColour(lightNode);
      addLight(getXMLPosition(lightNode),cast_shadows,range);
      lightNode = lightNode->next_sibling(LIGHT_STRING);
    }
    
    //Items
    rapidxml::xml_node<>* itemNode = root->first_node(ITEM_STRING);
    while(itemNode != NULL)
    {
      int id = boost::lexical_cast<int>(itemNode->first_attribute(ID_STRING)->value());
      addItem(id, getXMLPosition(itemNode), getXMLRotation(itemNode));
      itemNode = itemNode->next_sibling(ITEM_STRING);
    }

    //Monsters
    rapidxml::xml_node<>* monsterNode = root->first_node(MONSTER_STRING);
    while(monsterNode != NULL)
    {
      int id = boost::lexical_cast<int>(monsterNode->first_attribute(ID_STRING)->value());
      addMonster(id, getXMLPosition(monsterNode), getXMLRotation(monsterNode));
      monsterNode = monsterNode->next_sibling(MONSTER_STRING);
    }
    
    //Portals
    rapidxml::xml_node<>* portalNode = root->first_node(PORTAL_STRING);
    while(portalNode != NULL)
    {
      int id = boost::lexical_cast<int>(portalNode->first_attribute(ID_STRING)->value());
      int targetSceneID = boost::lexical_cast<int>(portalNode->first_attribute(TARGET_SCENE_ID_STRING)->value());
      int targetPortalID = boost::lexical_cast<int>(portalNode->first_attribute(TARGET_PORTAL_ID_STRING)->value());
      addPortal(new Portal(id, targetSceneID, targetPortalID, getXMLPosition(portalNode), getXMLVector(portalNode, LOOK_AT_X_STRING, LOOK_AT_Y_STRING, LOOK_AT_Z_STRING)));
      portalNode = portalNode->next_sibling(PORTAL_STRING);//"portal");
    }

    //Particles
    rapidxml::xml_node<>* particleNode = root->first_node(PARTICLE_STRING);//"particle");
    while(particleNode != NULL)
    {
      addParticles(particleNode->first_attribute(NAME_STRING)->value(), particleNode->first_attribute(TEMPLATE_NAME_STRING)->value(),getXMLPosition(particleNode));
      particleNode = particleNode->next_sibling(PARTICLE_STRING);
    }
  }
  catch (rapidxml::parse_error e)
  {
    std::stringstream ss;
    ss << "could not load the xml scene '" << file << "': " << e.what() << std::endl;
    throw NHException(ss.str().c_str());
  }
}
*/

//-------------------------------------------------------------------------------------
void Scene::build()
{
  scenePathfindManager->build();//note that this needs to be done before building architecture because architecture->build destroys the required scenenode
  sceneArchitectureManager->build();
}

//-------------------------------------------------------------------------------------
World* Scene::getWorld()
{
  return world;
}

//-------------------------------------------------------------------------------------
int Scene::getSceneID()
{
  return desc.id;
}
/*
//-------------------------------------------------------------------------------------
Vector3 Scene::getXMLVector(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third)
{
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  rapidxml::xml_attribute<>* xPosition = node->first_attribute(first.c_str());
  rapidxml::xml_attribute<>* yPosition = node->first_attribute(second.c_str());
  rapidxml::xml_attribute<>* zPosition = node->first_attribute(third.c_str());

  if(xPosition) x = boost::lexical_cast<float>(xPosition->value());
  if(yPosition) y = boost::lexical_cast<float>(yPosition->value());
  if(zPosition) z = boost::lexical_cast<float>(zPosition->value());

  return Vector3(x,y,z);
}

//-------------------------------------------------------------------------------------
Quaternion Scene::getXMLRotation(rapidxml::xml_node<>* node)
{
  Quaternion rotation = Quaternion::IDENTITY;
  Vector3 components = getXMLVector(node, ROTATION_X_STRING, ROTATION_Y_STRING, ROTATION_Z_STRING);
  
  rotation.FromAngleAxis(Ogre::Degree(components.x), Vector3::UNIT_X);
  rotation.FromAngleAxis(Ogre::Degree(components.y), Vector3::UNIT_Y);
  rotation.FromAngleAxis(Ogre::Degree(components.z), Vector3::UNIT_Z);

  return rotation;
}

//-------------------------------------------------------------------------------------
Ogre::ColourValue Scene::getXMLColour(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third, std::string fourth)
{
  float a = 1.0f;
  rapidxml::xml_attribute<>* aColour = node->first_attribute(fourth.c_str());  
  if(aColour) a = boost::lexical_cast<float>(aColour->value());

  Vector3 components = getXMLVector(node, first, second, third);
 
  return Ogre::ColourValue(components.x,components.y,components.z,a);
}
  
//-------------------------------------------------------------------------------------
Vector3 Scene::getXMLScale(rapidxml::xml_node<>* node)
{
  return getXMLVector(node, SCALE_X_STRING, SCALE_Y_STRING, SCALE_Z_STRING);//"sx", "sy", "sz");
}

//-------------------------------------------------------------------------------------
Vector3 Scene::getXMLPosition(rapidxml::xml_node<>* node)
{
  return getXMLVector(node, TRANSLATION_X_STRING, TRANSLATION_Y_STRING, TRANSLATION_Z_STRING);//"tx", "ty", "tz");
}
*/

//-------------------------------------------------------------------------------------
Portal* Scene::getPortal(int id)
{
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it)
  {
    if((*it)->getID() == id) return (*it);
  }

  throw NHException("no portal was found with the given id in Scene::getPortal()");//temporarily throw exception
  return NULL;
}

//-------------------------------------------------------------------------------------
Portal* Scene::getDefaultPortal()
{
  return getPortal(0);//could make more intelligent
}

//-------------------------------------------------------------------------------------
void Scene::destroyAllAttachedMoveables(Ogre::SceneNode* node)
{
  if(!node) return;

  // Destroy all the attached objects
  Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

  while (itObject.hasMoreElements()) node->getCreator()->destroyMovableObject(itObject.getNext());

  // Recurse to child SceneNodes
  Ogre::SceneNode::ChildNodeIterator itChild = node->getChildIterator();

  while (itChild.hasMoreElements())
  {
    Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
    destroyAllAttachedMoveables(pChildNode);
  }
}

//-------------------------------------------------------------------------------------
void Scene::destroySceneNode(Ogre::SceneNode* node)
{
  if(!node) return;
  destroyAllAttachedMoveables(node);
  node->removeAndDestroyAllChildren();
  node->getCreator()->destroySceneNode(node);
}

//-------------------------------------------------------------------------------------
ArchitectureManager* Scene::getArchitectureManager()
{
  return sceneArchitectureManager;
}

//-------------------------------------------------------------------------------------
PathfindManager* Scene::getPathfindManager()
{
  return scenePathfindManager;
}

//-------------------------------------------------------------------------------------
void Scene::setShadowsEnabled(bool enabled)
{
  if(enabled) 
  {
    sceneGraphicsManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
    sceneGraphicsManager->setShadowColour(desc.shadowColour);
    sceneGraphicsManager->setShadowTextureSize(4096);//1024 seems very pixelated
    sceneGraphicsManager->setShadowTextureCount(5);
  }
  else
  {
    sceneGraphicsManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
  }
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawShadows(bool enabled)
{
  sceneGraphicsManager->setShowDebugShadows(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawBoundingBoxes(bool enabled)
{
  sceneGraphicsManager->showBoundingBoxes(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawNavigationMesh(bool enabled)
{
  scenePathfindManager->setDrawNavigationMesh(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setGravity(Vector3 gravity)
{
  scenePhysicsManager->setGravity(physx::PxVec3(gravity.x, gravity.y, gravity.z));
}

//-------------------------------------------------------------------------------------
Vector3 Scene::getGravity()
{
  physx::PxVec3 gravity = scenePhysicsManager->getGravity();
  return Vector3(gravity.x, gravity.y, gravity.z);
}

//-------------------------------------------------------------------------------------
void Scene::reset()
{
  release();
  setup();
}

//-------------------------------------------------------------------------------------
void Scene::setup()
{
  //static physx::PxDefaultSimulationFilterShader defaultFilterShader;//??

  //scene physics
  physx::PxSceneDesc physicsDesc(world->getPhysicsManager()->getTolerancesScale());
  physicsDesc.gravity = physx::PxVec3(desc.gravity.x, desc.gravity.y, desc.gravity.z);

  physicsDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(numberPhysicsCPUThreads);
  if(!physicsDesc.cpuDispatcher) throw NHException("could not create scene CPU dispatcher");

  physicsDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
  if(!physicsDesc.filterShader) throw NHException("filter shader creation failed");

  scenePhysicsManager = world->getPhysicsManager()->getPhysics()->createScene(physicsDesc);
  if(!scenePhysicsManager) throw NHException("could not create scene physics manager");

  sceneControllerManager = PxCreateControllerManager(*world->getPhysicsManager()->getFoundation());
  if(!sceneControllerManager) throw NHException("could not create scene controller manager");

  sceneGraphicsManager = world->getRoot()->createSceneManager(Ogre::ST_GENERIC);

  scenePathfindManager = new PathfindManager(sceneGraphicsManager);

  setShadowsEnabled(world->isShadowsEnabled());

  sceneArchitectureManager = new ArchitectureManager(this, scenePathfindManager);

  setAmbientColour(desc.ambientLight);

  SceneLoader().load(desc.file, this);//load an xml scene

  //flockTest.setScene(this);//boids flocking test

  if(localPlayer) addPlayer(localPlayer);


#ifdef _DEBUG
  scenePhysicsManager->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
  scenePhysicsManager->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
#endif
}

//-------------------------------------------------------------------------------------
void Scene::release()
{
  if(localPlayer) localPlayer->setScene(NULL);//detach player from this scene
  //localplayer to NULL?

  defaultEntry = NULL;

  delete scenePathfindManager;
  scenePathfindManager = NULL;

  delete sceneArchitectureManager;
  sceneArchitectureManager = NULL;

  lights.destroyAll();
  monsters.destroyAll();
  items.destroyAll();
  interactives.destroyAll();
  players.destroyAll();

  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it)
  {
    delete (*it);
  }
  portals.clear();
  
  if(scenePhysicsManager) scenePhysicsManager->release();
  scenePhysicsManager = NULL;

  if(sceneGraphicsManager) world->getRoot()->destroySceneManager(sceneGraphicsManager);
  sceneGraphicsManager = NULL;
}

//-------------------------------------------------------------------------------------
bool Scene::hasPlayer()
{
  return localPlayer != NULL;
}

//-------------------------------------------------------------------------------------
void Scene::setAmbientColour(Ogre::ColourValue colour)
{
  sceneGraphicsManager->setAmbientLight(colour);
}