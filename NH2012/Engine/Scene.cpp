#include "Scene.h"

#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSimpleFactory.h"

#include "World.h"
#include "Player.h."
#include "Monster.h"
#include "Item.h"
#include "Architecture.h"
#include "Portal.h"
#include "Flock.h"

//-------------------------------------------------------------------------------------
Scene::Scene(World* world, int id)
  : world(world),
    id(id),
    sceneManager(world->getRoot()->createSceneManager(Ogre::ST_GENERIC)),
    controllerManager(NULL),
    physicsManager(NULL),
    instanceNumber(0),
    defaultEntry(NULL),
    player(NULL),
    defaultAmbientColour(0.1f,0.1f,0.1f),
    numberPhysicsCPUThreads(4),
    stepSize(1.0 / 60.0),
    accumulator(0.0),
    active(false),
    //flockTest(100),
    totalElapsed(0.0),
    particles(),
    monsters(),
    portals(),
    lights(),
    items()
{
  //static physx::PxDefaultSimulationFilterShader defaultFilterShader;//??

  //scene physics
  physx::PxSceneDesc desc(world->getTolerancesScale());
  desc.gravity = physx::PxVec3(0.0f, world->gravity, 0.0f);
  
  desc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(numberPhysicsCPUThreads);
  if(!desc.cpuDispatcher) throw NHException("Could not create scene CPU dispatcher.");

  desc.filterShader = &physx::PxDefaultSimulationFilterShader;
  if(!desc.filterShader) throw NHException("Filter shader creation failed.");
  
  physicsManager = world->getPhysics()->createScene(desc);
  if(!physicsManager) throw NHException("Could not create scene physics manager.");

  controllerManager = PxCreateControllerManager(world->getPhysics()->getFoundation());
  if(!controllerManager) throw NHException("Could not create scene controller manager.");

  if(world->enableShadows) 
  {
    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE);
    sceneManager->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
    sceneManager->setShadowTextureSize(1024);
    sceneManager->setShadowTextureCount(5);
  }

  sceneManager->setShowDebugShadows(world->isDebug());
  sceneManager->showBoundingBoxes(world->isDebug());
  
  architecture = new Architecture(this);

  SceneDesc sceneDesc = world->getDataManager()->getScene(id);//getting scene information from the world data manager
  
  name = sceneDesc.name;
  load(sceneDesc.file);//loading the scene file

  sceneManager->setAmbientLight(defaultAmbientColour);

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

  //ivyTestNoDestruction = new PlantOgreMesh(sceneManager, PlantDesc(), Ogre::Vector3(0,0,0));

  //sceneManager->getRootSceneNode()->attachObject(sceneManager->createEntity("theatre_ivy.mesh"));//static ivy mesh

  //building static geometry
  architecture->build();

  //flockTest.setScene(this);//boids flocking test
}

//-------------------------------------------------------------------------------------
Scene::~Scene(void)
{
  //Deleting architecture
  if(architecture) delete architecture;
  architecture = NULL;

  //Deleting all monsters
  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)  
  {
    //world->releaseMonster(*it);
    if(*it) delete (*it);
    (*it) = NULL;
  }

  //Deleting all items
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it)
  {
    //world->releaseItem(*it);
    if(*it) delete (*it);
    (*it) = NULL;
  }

  //Deleting all portals
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it)
  {
    if(*it) delete (*it);
    (*it) = NULL;
  }

  //Releasing physics
  physicsManager->release();
  physicsManager = NULL;

  //Releasing scene manager
  world->getRoot()->destroySceneManager(sceneManager);
  sceneManager = NULL;
}

//-------------------------------------------------------------------------------------
bool Scene::isActive()
{
  return active;
}

//-------------------------------------------------------------------------------------
Ogre::String Scene::getName()
{
  return name;
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
  active = true;
  Ogre::Vector3 position;
  Ogre::Vector3 lookAt;
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
    position = Ogre::Vector3::ZERO;
    lookAt = Ogre::Vector3::ZERO;
  }

  Scene* old = player->getScene();
  if(old) old->removePlayer(player);//need to remove player from previous scene
  player->setScene(this, position, lookAt);
  this->player = player;
}

//-------------------------------------------------------------------------------------
void Scene::addMonster(int id, Ogre::Vector3 position, Ogre::Quaternion rotation)
{
  Monster* monster = new Monster(world->getDataManager()->getMonster(id));//this->getWorld()->createMonster(id);
  monster->setPosition(position);
  monster->setScene(this);
  monsters.push_back(monster);
}

//-------------------------------------------------------------------------------------
void Scene::addItem(int id, Ogre::Vector3 position, Ogre::Quaternion rotation)
{
  Item* item = new Item(world->getDataManager()->getItem(id));//this->getWorld()->createItem(id);
  item->setPosition(position);
  item->setScene(this);
  items.push_back(item);
}

//-------------------------------------------------------------------------------------
void Scene::addLight(Ogre::Vector3 position, bool castShadows, Ogre::Real range, Ogre::ColourValue colour)
{
  Ogre::Light* light = sceneManager->createLight("light" + Ogre::StringConverter::toString(getNewInstanceNumber()));
  lights.push_back(light);
  light->setPosition(position);
  light->setAttenuation(range, 0.95f, 0.05f, 0);
  light->setDiffuseColour(colour);
  light->setSpecularColour(Ogre::ColourValue::White);
  light->setCastShadows(castShadows);

  //Same clipping as the camera set up
  //light->setShadowNearClipDistance(0.4f);
  //light->setShadowFarClipDistance(400.0f);
  //light->setShadowFarDistance(400.0f);
}

//-------------------------------------------------------------------------------------
void Scene::addParticles(Ogre::String name, Ogre::String templateName, Ogre::Vector3 position, Ogre::Real speed)
{
  Ogre::ParticleSystem* particle = sceneManager->createParticleSystem(name, templateName);//"Rain");//, "Examples/Rain");
  particle->fastForward(speed);
  Ogre::SceneNode* particleNode = sceneManager->getRootSceneNode()->createChildSceneNode("particle" + Ogre::StringConverter::toString(getNewInstanceNumber()));
  particleNode->setPosition(position);
  particleNode->attachObject(particle);
  particles.push_back(particle);
}

//-------------------------------------------------------------------------------------
void Scene::addPortal(Portal* portal)
{
  portals.push_back(portal);
}

//-------------------------------------------------------------------------------------
void Scene::removePlayer(Player* player)
{
  if(this->player == player) 
  {
    this->player = NULL;
    active = false;//when more than one player, will need to check all players before deactivating scene
  }
}

//-------------------------------------------------------------------------------------
int Scene::getNewInstanceNumber()
{
  instanceNumber++;
  return instanceNumber;
}

//-------------------------------------------------------------------------------------
void Scene::update(double elapsedSeconds)
{
  if(!physicsManager) throw NHException("No physics found. Scene::frameRenderingQueued()");
  
  physicsManager->fetchResults(true);
  
  architecture->update(elapsedSeconds);//only really needed for pathfinding debug display

  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) (*it)->update(elapsedSeconds);//iterate monsters
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) (*it)->update(elapsedSeconds);//iterate items
  
  if(player) player->update(elapsedSeconds);
  
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it) 
  {
    if((*it)->isLoadRequired(player->getPosition())) world->loadScene((*it)->getID());
  }

  totalElapsed += elapsedSeconds;
  //flockTest.update(totalElapsed);//evt.timeSinceLastFrame);

  //ivyTestNoDestruction->grow(elapsedSeconds);

  if(!world->freezeCollisionDebug) advancePhysics(elapsedSeconds);
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
#define SCENE_STRING "scene"
#define AMBIENT_RED_STRING "ambient_r"
#define AMBIENT_GREEN_STRING "ambient_g"
#define AMBIENT_BLUE_STRING "ambient_b"
#define NORTH_STRING "north"
#define ARCHITECTURE_STRING "architecture"
#define ID_STRING "id"
#define LIGHT_STRING "light"
#define CAST_SHADOWS_STRING "cast_shadows"
#define RANGE_STRING "range"
#define ITEM_STRING "item"
#define PORTAL_STRING "portal"
#define TARGET_SCENE_ID_STRING "target_scene_id"
#define TARGET_PORTAL_ID_STRING "target_portal_id"
#define LOOK_AT_X_STRING "ltx"
#define LOOK_AT_Y_STRING "lty"
#define LOOK_AT_Z_STRING "ltz"
#define PARTICLE_STRING "particle"
#define NAME_STRING "name"
#define TEMPLATE_NAME_STRING "template_name"

#define TRANSLATION_X_STRING "tx"
#define TRANSLATION_Y_STRING "ty"
#define TRANSLATION_Z_STRING "tz"
#define ROTATION_X_STRING "rx"
#define ROTATION_Y_STRING "ry"
#define ROTATION_Z_STRING "rz"
#define SCALE_X_STRING "sx"
#define SCALE_Y_STRING "sy"
#define SCALE_Z_STRING "sz"

//-------------------------------------------------------------------------------------
void Scene::load(std::string file)
{
  try
  {
    std::ifstream streamfile(file);
    rapidxml::xml_document<> doc;

    std::vector<char> buffer((std::istreambuf_iterator<char>(streamfile)), std::istreambuf_iterator<char>());

    buffer.push_back('\0');//null terminating the buffer

    //std::cout << &buffer[0] << std::endl; /*test the buffer */

    doc.parse<0>(&buffer[0]);
    rapidxml::xml_node<>* root = doc.first_node(SCENE_STRING);//"scene");

    defaultAmbientColour = getXMLColour(root, AMBIENT_RED_STRING, AMBIENT_GREEN_STRING, AMBIENT_BLUE_STRING);//"ambient_r", "ambient_g", "ambient_b");

    
    //description attributes
    north = boost::lexical_cast<float>(root->first_attribute(NORTH_STRING)->value());
   
    //Architecture
    rapidxml::xml_node<>* architectureNode = root->first_node(ARCHITECTURE_STRING);//"architecture");
    while(architectureNode != NULL)
    {
      int id = boost::lexical_cast<int>(architectureNode->first_attribute(ID_STRING)->value());
      architecture->add(world->getDataManager()->getArchitecture(id).mesh, getXMLPosition(architectureNode), getXMLRotation(architectureNode), getXMLScale(architectureNode));
      architectureNode = architectureNode->next_sibling(ARCHITECTURE_STRING);
    }
   
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
    
    //Portals
    rapidxml::xml_node<>* portalNode = root->first_node(PORTAL_STRING);
    while(portalNode != NULL)
    {
      int id = boost::lexical_cast<int>(portalNode->first_attribute(ID_STRING)->value());
      int targetSceneID = boost::lexical_cast<int>(portalNode->first_attribute(TARGET_SCENE_ID_STRING)->value());
      int targetPortalID = boost::lexical_cast<int>(portalNode->first_attribute(TARGET_PORTAL_ID_STRING)->value());
      addPortal(new Portal(id, targetSceneID, targetPortalID, getXMLPosition(portalNode), getXMLVector(portalNode, LOOK_AT_X_STRING, LOOK_AT_Y_STRING, LOOK_AT_Z_STRING)));
      if(defaultEntry == NULL && portals.size() > 0) defaultEntry = portals[0];//creating a default entry point
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
    std::cout << "Could not load the xml scene file at " << file << ". " << e.what() << std::endl;
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
  return id;
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Scene::getXMLVector(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third)
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

  return Ogre::Vector3(x,y,z);
}

//-------------------------------------------------------------------------------------
Ogre::Quaternion Scene::getXMLRotation(rapidxml::xml_node<>* node)
{
  Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
  Ogre::Vector3 components = getXMLVector(node, ROTATION_X_STRING, ROTATION_Y_STRING, ROTATION_Z_STRING);
  
  rotation.FromAngleAxis(Ogre::Degree(components.x), Ogre::Vector3::UNIT_X);
  rotation.FromAngleAxis(Ogre::Degree(components.y), Ogre::Vector3::UNIT_Y);
  rotation.FromAngleAxis(Ogre::Degree(components.z), Ogre::Vector3::UNIT_Z);

  return rotation;
}

//-------------------------------------------------------------------------------------
Ogre::ColourValue Scene::getXMLColour(rapidxml::xml_node<>* node, std::string first, std::string second, std::string third, std::string fourth)
{
  float a = 1.0f;
  rapidxml::xml_attribute<>* aColour = node->first_attribute(fourth.c_str());  
  if(aColour) a = boost::lexical_cast<float>(aColour->value());

  Ogre::Vector3 components = getXMLVector(node, first, second, third);
 
  return Ogre::ColourValue(components.x,components.y,components.z,a);
}
  
//-------------------------------------------------------------------------------------
Ogre::Vector3 Scene::getXMLScale(rapidxml::xml_node<>* node)
{
  return getXMLVector(node, SCALE_X_STRING, SCALE_Y_STRING, SCALE_Z_STRING);//"sx", "sy", "sz");
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Scene::getXMLPosition(rapidxml::xml_node<>* node)
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

  throw NHException("No portal was found with the given id in 'Scene::getPortal(int)'.");//temporarily throw exception
  return NULL;
}

//-------------------------------------------------------------------------------------
Portal* Scene::getDefaultPortal()
{
  return getPortal(0);//could make more intelligent
}


