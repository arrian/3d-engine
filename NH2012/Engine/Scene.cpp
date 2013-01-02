#include "Scene.h"

#include <algorithm>

#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSimpleFactory.h"

#include "World.h"
#include "Player.h."
#include "Monster.h"
#include "Item.h"
#include "Light.h"
#include "Architecture.h"
#include "Portal.h"
#include "Flock.h"
#include "Door.h"
#include "Chest.h"

//-------------------------------------------------------------------------------------
Scene::Scene(SceneDesc desc, World* world)
  : world(world),
    desc(desc),
    sceneManager(NULL),
    controllerManager(NULL),
    physicsManager(NULL),
    defaultEntry(NULL),
    player(NULL),
    numberPhysicsCPUThreads(4),
    stepSize(1.0 / 60.0),
    accumulator(0.0),
    pathfinder(NULL),
    particles(),
    monsters(),
    portals(),
    lights(),
    items()
{
  setup();
}

//-------------------------------------------------------------------------------------
Scene::~Scene(void)
{
  release();
}

//-------------------------------------------------------------------------------------
Ogre::String Scene::getName()
{
  return desc.name;
}

//-------------------------------------------------------------------------------------
Ogre::SceneManager* Scene::getSceneManager()
{
  return sceneManager;
}

//-------------------------------------------------------------------------------------
physx::PxScene* Scene::getPhysicsManager()
{
  return physicsManager;
}

//-------------------------------------------------------------------------------------
physx::PxControllerManager* Scene::getControllerManager()
{
  return controllerManager;
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
  this->player = player;
}

//-------------------------------------------------------------------------------------
void Scene::addMonster(int id, Vector3 position, Quaternion rotation)
{
  Monster* monster = new Monster(world->getDataManager()->getMonster(id));
  monster->setScene(this);
  monster->setPosition(position);
  monster->setGoal(new Go(getPathfindManager()->getRandomNavigablePoint(), Priority::HIGH));
  monsters.push_back(monster);
}

//-------------------------------------------------------------------------------------
void Scene::addItem(int id, Vector3 position, Quaternion rotation)
{
  Item* item = new Item(world->getDataManager()->getItem(id));//this->getWorld()->createItem(id);
  item->setScene(this);
  item->setPosition(position);
  item->setRotation(rotation);
  items.push_back(item);
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
  interactives.push_back(interactive);
}

//-------------------------------------------------------------------------------------
void Scene::addLight(Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour)
{
  Light* light = new Light(this, position, castShadows, range, colour);
  lights.push_back(light);
}

//-------------------------------------------------------------------------------------
void Scene::addParticles(Ogre::String name, Ogre::String templateName, Vector3 position, Ogre::Real speed)
{
  Ogre::ParticleSystem* particle = sceneManager->createParticleSystem(name, templateName);//"Rain");//, "Examples/Rain");
  particle->setSpeedFactor(speed);
  Ogre::SceneNode* particleNode = sceneManager->getRootSceneNode()->createChildSceneNode();//"particle" + Ogre::StringConverter::toString(getNewInstanceNumber()));
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
  if(this->player == player) this->player = NULL;
}

//-------------------------------------------------------------------------------------
void Scene::removeItem(Item* item)
{
  std::vector<Item*>::iterator itemIter = std::find(items.begin(), items.end(), item);
  if(itemIter == items.end()) throw NHException("could not find the item");

  delete item;
  items.erase(itemIter);
}

//-------------------------------------------------------------------------------------
void Scene::removeMonster(Monster* monster)
{
  std::vector<Monster*>::iterator monsterIter = std::find(monsters.begin(), monsters.end(), monster);
  if(monsterIter == monsters.end()) throw NHException("could not find the monster");

  delete monster;
  monsters.erase(monsterIter);  
}

//-------------------------------------------------------------------------------------
void Scene::update(double elapsedSeconds)
{
  if(!physicsManager) throw NHException("no physics found in Scene::update()");
  
  physicsManager->fetchResults(true);
  
  pathfinder->update(elapsedSeconds);
  architecture->update(elapsedSeconds);//only really needed for pathfinding debug display... not needed anymore?

  for(std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it) (*it)->update(elapsedSeconds);//iterate lights
  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) (*it)->update(elapsedSeconds);//iterate monsters
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) (*it)->update(elapsedSeconds);//iterate items
  
  if(player) player->update(elapsedSeconds);
  
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it) 
  {
    if((*it)->isLoadRequired(player->getPosition())) world->loadScene((*it)->getID());
  }

  if(world->isPhysicsEnabled()) advancePhysics(elapsedSeconds);
}

//-------------------------------------------------------------------------------------
bool Scene::advancePhysics(double elapsedSeconds)
{
  accumulator += elapsedSeconds;
  if(accumulator < stepSize) return false;
  accumulator -= stepSize;
  physicsManager->simulate((float) stepSize);

  return true;
}


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

    //std::cout << &buffer[0] << std::endl; /*test the buffer */

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
      architecture->add(world->getDataManager()->getArchitecture(id), getXMLPosition(architectureNode), getXMLRotation(architectureNode), getXMLScale(architectureNode));
      architectureNode = architectureNode->next_sibling(ARCHITECTURE_STRING);
    }

    //building static geometry - must do here else monster placement suffers
    pathfinder->build();//note that this needs to be done before building architecture because architecture->build destroys the required scenenode
    architecture->build();
   
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
Architecture* Scene::getArchitecture()
{
  return architecture;
}

//-------------------------------------------------------------------------------------
PathfindManager* Scene::getPathfindManager()
{
  return pathfinder;
}

//-------------------------------------------------------------------------------------
void Scene::setShadowsEnabled(bool enabled)
{
  if(enabled) 
  {
    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
    sceneManager->setShadowColour(desc.shadowColour);
    sceneManager->setShadowTextureSize(4096);//1024 seems very pixelated
    sceneManager->setShadowTextureCount(5);
  }
  else
  {
    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE);
  }
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawShadows(bool enabled)
{
  sceneManager->setShowDebugShadows(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawBoundingBoxes(bool enabled)
{
  sceneManager->showBoundingBoxes(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setDebugDrawNavigationMesh(bool enabled)
{
  pathfinder->setDrawNavigationMesh(enabled);
}

//-------------------------------------------------------------------------------------
void Scene::setGravity(Vector3 gravity)
{
  physicsManager->setGravity(physx::PxVec3(gravity.x, gravity.y, gravity.z));
}

//-------------------------------------------------------------------------------------
Vector3 Scene::getGravity()
{
  physx::PxVec3 gravity = physicsManager->getGravity();
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

  physicsManager = world->getPhysicsManager()->getPhysics()->createScene(physicsDesc);
  if(!physicsManager) throw NHException("could not create scene physics manager");

  controllerManager = PxCreateControllerManager(*world->getPhysicsManager()->getFoundation());
  if(!controllerManager) throw NHException("could not create scene controller manager");

  sceneManager = world->getRoot()->createSceneManager(Ogre::ST_GENERIC);

  pathfinder = new PathfindManager(sceneManager);

  setShadowsEnabled(world->isShadowsEnabled());

  architecture = new Architecture(this, pathfinder);

  load(desc.file);//loading the scene file

  sceneManager->setAmbientLight(desc.ambientLight);

  /*
  //static spotlight
  Ogre::Light* light = sceneManager->createLight();
  light->setType(Ogre::Light::LT_SPOTLIGHT);
  light->setPosition(0,1,0);
  light->setDirection(0,-1,0);
  light->setSpotlightInnerAngle(Ogre::Degree(25));
  light->setSpotlightOuterAngle(Ogre::Degree(45));
  light->setAttenuation(30,0,0,0);
  light->setDiffuseColour(Ogre::ColourValue::White);
  light->setCastShadows(true);
  */

  //flockTest.setScene(this);//boids flocking test

  if(player) addPlayer(player);
}

//-------------------------------------------------------------------------------------
void Scene::release()
{
  if(player) player->setScene(NULL);//detach player from this scene

  defaultEntry = NULL;

  if(pathfinder) delete pathfinder;
  pathfinder = NULL;

  if(architecture) delete architecture;
  architecture = NULL;

  for(std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
  {
    if(*it) delete (*it);
    (*it) = NULL;
  }
  lights.clear();

  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)  
  {
    if(*it) delete (*it);
    (*it) = NULL;
  }
  monsters.clear();

  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it)
  {
    if(*it) delete (*it);
    (*it) = NULL;
  }
  items.clear();

  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it)
  {
    if(*it) delete (*it);
    (*it) = NULL;
  }
  portals.clear();
  

  if(physicsManager) physicsManager->release();
  physicsManager = NULL;

  if(sceneManager) world->getRoot()->destroySceneManager(sceneManager);
  sceneManager = NULL;
}

//-------------------------------------------------------------------------------------
bool Scene::hasPlayer()
{
  return player != NULL;
}

