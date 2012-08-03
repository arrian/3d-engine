#include "Scene.h"

#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSimpleFactory.h"


Scene::Scene(World* world, Ogre::String name, SceneType type)
  : world(world),
    sceneManager(world->getRoot()->createSceneManager(Ogre::ST_INTERIOR)),
    physicsManager(0),
    controllerManager(0),
    instanceNumber(0),
    name(name),
    type(type),
    monsters(),
    items(),
    lights(),
    particles(),
    player(0),
    active(false)
{
  //static physx::PxDefaultSimulationFilterShader defaultFilterShader;//??

  //scene physics
  physx::PxSceneDesc desc(world->getTolerancesScale());
  desc.gravity = physx::PxVec3(0.0f, world->gravity, 0.0f);
  
  desc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(8);
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

  load(name);//loading the scene file
  
  /*
  switch(type)
  {
    case PREDEFINED: generatePredefined(); break;
    case FILE_XML: loadXmlLevel(name);break;
    default: break;
  }*/

  if(world->enableLights) sceneManager->setAmbientLight(Ogre::ColourValue(0.053f,0.05f,0.05f));
  else sceneManager->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));

  //building static geometry
  architecture->build();
}

Scene::~Scene(void)
{

  if(architecture) delete architecture;
  architecture = 0;

  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)  
  {
    world->releaseMonster(*it);
    if(*it) delete (*it);
    (*it) = 0;
  }

  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it)
  {
    world->releaseItem(*it);
    if(*it) delete (*it);
    (*it) = 0;
  }

  physicsManager->release();
  physicsManager = 0;

  world->getRoot()->destroySceneManager(sceneManager);
  sceneManager = 0;
}

bool Scene::isActive()
{
  return active;
}

Ogre::String Scene::getName()
{
  return name;
}

Ogre::SceneManager* Scene::getSceneManager()
{
  return sceneManager;
}

physx::PxScene* Scene::getPhysicsManager()
{
  return physicsManager;
}

physx::PxControllerManager* Scene::getControllerManager()
{
  return controllerManager;
}

void Scene::addPlayer(Player* player)
{
  active = true;
  player->setScene(this, Ogre::Vector3(0,10,0), Ogre::Vector3(800,50,600));
  this->player = player;
}

void Scene::addMonster(Ogre::Vector3 position)
{
  Monster* monster = this->getWorld()->createMonster();
  monster->setPosition(position);
  monster->setScene(this);
  monsters.push_back(monster);
}

//TODO use references rather than pointers
void Scene::addItem(Ogre::Vector3 position)
{
  Item* item = this->getWorld()->createItem();
  item->setPosition(position);
  item->setScene(this);
  items.push_back(item);
}

void Scene::addLight(Ogre::Vector3 position, bool castShadows, Ogre::Real range)
{
  Ogre::Light* light = sceneManager->createLight("light" + Ogre::StringConverter::toString(getNewInstanceNumber()));
  lights.push_back(light);
  light->setPosition(position);
  light->setAttenuation(range, 1.0f, 0.0014f, 0.000007f);
  light->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 0.95f));
  light->setCastShadows(castShadows);
}

void Scene::addParticles(Ogre::String name, Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::Real speed)
{
  Ogre::ParticleSystem* particle = sceneManager->createParticleSystem(name);//"Rain");//, "Examples/Rain");
  particle->fastForward(speed);
  Ogre::SceneNode* particleNode = sceneManager->getRootSceneNode()->createChildSceneNode("particle" + Ogre::StringConverter::toString(getNewInstanceNumber()));
  particleNode->setPosition(position);
  particleNode->setScale(scale);
  particleNode->attachObject(particle);
  particles.push_back(particle);
}

void Scene::removePlayer(Player* player)
{
  if(this->player == player) 
  {
    this->player = 0;
    active = false;//when more than one player, will need to check all players before deactivating scene
  }
}

int Scene::getNewInstanceNumber()
{
  instanceNumber++;
  return instanceNumber;
}

float accumulator = 0.0f;
float stepSize = 1.0f / 60.0f;

void Scene::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!physicsManager) std::cout << "no physics found for this frame" << std::endl;
  
  physicsManager->fetchResults(true);
  if(player) player->frameRenderingQueued(evt);

  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) (*it)->frameRenderingQueued(evt);//iterate monsters
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) (*it)->frameRenderingQueued(evt);//iterate items

  if(!world->freezeCollisionDebug) advancePhysics(evt.timeSinceLastFrame);
}

bool Scene::advancePhysics(Ogre::Real dt)
{
  accumulator += dt;
  if(accumulator < stepSize) return false;
  accumulator -= stepSize;
  physicsManager->simulate(stepSize);

  return true;
}

/*
void Scene::generatePredefined()
{
  for(int i = 106; i <= 123; i++) architecture->add(world->getDataManager()->getArchitecture(i)->mesh);

  addLight(Ogre::Vector3(0,10,0),false,50);
  addLight(Ogre::Vector3(0,10,-30),false,50);
  addLight(Ogre::Vector3(10,10,-80),false,50);
  
  //architecture->add(world->getDataManager()->getArchitecture(34)->mesh);
  /*
  for(int i = 19; i <= 32; i++) architecture->add(world->getDataManager()->getArchitecture(i)->mesh);
  architecture->add(world->getDataManager()->getArchitecture(33)->mesh, Ogre::Vector3(-380.0f, 0.0f, 0.0f));

  for(int i = 0; i < 10; i++)
  {
    architecture->add(world->getDataManager()->getArchitecture(47)->mesh, Ogre::Vector3(-445.0f - 100.0f * i, 0.0f, 0.0f));
  }
  */
}*/

void Scene::load(std::string file)
{
   std::ifstream myfile(file);
   rapidxml::xml_document<> doc;

   /* "Read file into vector<char>"  See linked thread above*/
   std::vector<char> buffer((std::istreambuf_iterator<char>(myfile)), std::istreambuf_iterator<char>());

   buffer.push_back('\0');

   std::cout << &buffer[0] << endl; /*test the buffer */

   doc.parse<0>(&buffer[0]); 

   cout << "Name of my first node is: " << doc.first_node()->name() << "\n";  /*test the xml_document */
}

World* Scene::getWorld()
{
  return world;
}

int Scene::getSceneID()
{
  throw NHException("get scene id not implemented.");
  return 0;
}





