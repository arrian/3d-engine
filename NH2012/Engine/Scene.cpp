#include "Scene.h"

#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSimpleFactory.h"

//-------------------------------------------------------------------------------------
Scene::Scene(World* world, int id)
  : world(world),
    id(id),
    sceneManager(world->getRoot()->createSceneManager(Ogre::ST_INTERIOR)),
    physicsManager(0),
    controllerManager(0),
    instanceNumber(0),
    monsters(),
    items(),
    lights(),
    particles(),
    portals(),
    defaultEntry(0),
    player(0),
    active(false),
    accumulator(0.0f),
    stepSize(1.0f / 60.0f),
    defaultAmbientColour(0.8f,0.8f,0.8f),
    numberPhysicsCPUThreads(8),
    flockTest(40)
{
  //static physx::PxDefaultSimulationFilterShader defaultFilterShader;//??

  //scene physics
  physx::PxSceneDesc desc(world->getTolerancesScale());
  desc.gravity = physx::PxVec3(0.0f, world->gravity, 4.0f);
  
  desc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(numberPhysicsCPUThreads);
  if(!desc.cpuDispatcher) throw NHException("Could not create scene CPU dispatcher.");

  desc.filterShader = &physx::PxDefaultSimulationFilterShader;
  if(!desc.filterShader) throw NHException("Filter shader creation failed.");
  
  physicsManager = world->getPhysics()->createScene(desc);
  if(!physicsManager) throw NHException("Could not create scene physics manager.");

  controllerManager = PxCreateControllerManager(world->getPhysics()->getFoundation());
  if(!controllerManager) throw NHException("Could not create scene controller manager.");

  if(world->enableShadows) sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  sceneManager->setDisplaySceneNodes(world->isDebug());
  sceneManager->setShowDebugShadows(world->isDebug());
  sceneManager->showBoundingBoxes(world->isDebug());
  
  architecture = new Architecture(this);

  SceneDesc* sceneDesc = world->getDataManager()->getScene(id);//getting scene information from the world data manager
 
  if(sceneDesc == 0) 
  {
    throw NHException("Could not find the scene with the given id.");
    name = "Error_Scene";
  }
  else
  {
    name = sceneDesc->name;
    load(sceneDesc->file);//loading the scene file
  }

  sceneManager->setAmbientLight(defaultAmbientColour);
  
  //building static geometry
  architecture->build();
}

//-------------------------------------------------------------------------------------
Scene::~Scene(void)
{
  //Deleting architecture
  if(architecture) delete architecture;
  architecture = 0;

  //Deleting all monsters
  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)  
  {
    world->releaseMonster(*it);
    if(*it) delete (*it);
    (*it) = 0;
  }

  //Deleting all items
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it)
  {
    world->releaseItem(*it);
    if(*it) delete (*it);
    (*it) = 0;
  }

  //Deleting all portals
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it)
  {
    if(*it) delete (*it);
    (*it) = 0;
  }


  //Releasing physics
  physicsManager->release();
  physicsManager = 0;

  //Releasing scene manager
  world->getRoot()->destroySceneManager(sceneManager);
  sceneManager = 0;
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
  if(defaultEntry != 0 && portalID == -1)//use default portal
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
  player->setScene(this, position, lookAt);
  this->player = player;
}

//-------------------------------------------------------------------------------------
void Scene::addMonster(int id, Ogre::Vector3 position, Ogre::Quaternion rotation)
{
  Monster* monster = this->getWorld()->createMonster(id);
  monster->setPosition(position);
  monster->setScene(this);
  monsters.push_back(monster);
}

//-------------------------------------------------------------------------------------
void Scene::addItem(int id, Ogre::Vector3 position, Ogre::Quaternion rotation)
{
  Item* item = this->getWorld()->createItem(id);
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
  light->setAttenuation(range, 1.0f, 0.0014f, 0.000007f);
  light->setDiffuseColour(colour);
  light->setCastShadows(castShadows);
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
    this->player = 0;
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
void Scene::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!physicsManager) throw NHException("No physics found. Scene::frameRenderingQueued()");
  
  physicsManager->fetchResults(true);
  
  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) (*it)->frameRenderingQueued(evt);//iterate monsters
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) (*it)->frameRenderingQueued(evt);//iterate items
  
  if(player) player->frameRenderingQueued(evt);
  
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it) 
  {
    if((*it)->isLoadRequired(player->getPosition())) world->loadScene((*it)->getID());
  }

  flockTest.update(evt.timeSinceLastFrame);

  if(!world->freezeCollisionDebug) advancePhysics(evt.timeSinceLastFrame);
}

//-------------------------------------------------------------------------------------
bool Scene::advancePhysics(Ogre::Real dt)
{
  accumulator += dt;
  if(accumulator < stepSize) return false;
  accumulator -= stepSize;
  physicsManager->simulate(stepSize);

  return true;
}

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
    rapidxml::xml_node<>* root = doc.first_node("scene");

    defaultAmbientColour = getXMLColour(root, "ambient_r", "ambient_g", "ambient_b");

    
    //description attributes
    north = boost::lexical_cast<float>(root->first_attribute("north")->value());
   
    //Architecture
    rapidxml::xml_node<>* architectureNode = root->first_node("architecture");
    while(architectureNode != 0)
    {
      int id = boost::lexical_cast<int>(architectureNode->first_attribute("id")->value());
      architecture->add(world->getDataManager()->getArchitecture(id)->mesh, getXMLPosition(architectureNode), getXMLRotation(architectureNode), getXMLScale(architectureNode));
      architectureNode = architectureNode->next_sibling("architecture");
    }
   
    //Lights
    rapidxml::xml_node<>* lightNode = root->first_node("light");
    while(lightNode != 0)
    {
      bool cast_shadows = boost::lexical_cast<bool>(lightNode->first_attribute("cast_shadows")->value());
      float range = boost::lexical_cast<float>(lightNode->first_attribute("range")->value());
      Ogre::ColourValue colour = getXMLColour(lightNode);
      addLight(getXMLPosition(lightNode),cast_shadows,range);
      lightNode = lightNode->next_sibling("light");
    }
    
    //Items
    rapidxml::xml_node<>* itemNode = root->first_node("item");
    while(itemNode != 0)
    {
      int id = boost::lexical_cast<int>(itemNode->first_attribute("id")->value());
      addItem(id, getXMLPosition(itemNode), getXMLRotation(itemNode));
      itemNode = itemNode->next_sibling("item");
    }    
    
    //Portals
    rapidxml::xml_node<>* portalNode = root->first_node("portal");
    while(portalNode != 0)
    {
      int id = boost::lexical_cast<int>(portalNode->first_attribute("id")->value());
      int targetSceneID = boost::lexical_cast<int>(portalNode->first_attribute("target_scene_id")->value());
      int targetPortalID = boost::lexical_cast<int>(portalNode->first_attribute("target_portal_id")->value());
      addPortal(new Portal(id, targetSceneID, targetPortalID, getXMLPosition(portalNode), getXMLVector(portalNode, "ltx", "lty", "ltz")));
      if(defaultEntry == 0 && portals.size() > 0) defaultEntry = portals[0];//creating a default entry point
      portalNode = portalNode->next_sibling("portal");
    }

    //Particles
    rapidxml::xml_node<>* particleNode = root->first_node("particle");
    while(particleNode != 0)
    {
      addParticles(particleNode->first_attribute("name")->value(), particleNode->first_attribute("template_name")->value(),getXMLPosition(particleNode));
      particleNode = particleNode->next_sibling("particle");
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
  Ogre::Vector3 components = getXMLVector(node, "rx", "ry", "rz");
  
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
  return getXMLVector(node, "sx", "sy", "sz");
}

//-------------------------------------------------------------------------------------
Ogre::Vector3 Scene::getXMLPosition(rapidxml::xml_node<>* node)
{
  return getXMLVector(node, "tx", "ty", "tz");
}

//-------------------------------------------------------------------------------------
Portal* Scene::getPortal(int id)
{
  for(std::vector<Portal*>::iterator it = portals.begin(); it != portals.end(); ++it)
  {
    if((*it)->getID() == id) return (*it);
  }

  throw NHException("No portal was found with the given id in 'Scene::getPortal(int)'.");//temporarily throw exception
  return 0;
}

//-------------------------------------------------------------------------------------
Portal* Scene::getDefaultPortal()
{
  return getPortal(0);//could make more intelligent
}


