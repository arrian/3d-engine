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
    active(false),
    addItems(false),
    genRadius(500),
    genAngle(0),
    genXOrigin(800),
    genYOrigin(200)
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

  switch(type)
  {
  case PREDEFINED: generatePredefined(); break;
  case DUNGEON: generateDungeon(); break;
  case FILE_CHAR: loadCharLevel(name); break;
  default: break;
  }

  if(world->enableLights) sceneManager->setAmbientLight(Ogre::ColourValue(0.053f,0.05f,0.05f));
  else sceneManager->setAmbientLight(Ogre::ColourValue(1.0f,1.0f,1.0f));

  //building static geometry
  architecture->build();

  //temporary ground plane
  physx::PxRigidStatic* plane = PxCreatePlane(*world->getPhysics(), physx::PxPlane(physx::PxVec3(0,1,0), 0), *world->getDefaultPhysicsMaterial());
  physicsManager->addActor(*plane);

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
  player->setScene(this, Ogre::Vector3(800,80,200), Ogre::Vector3(800,50,600));
  this->player = player;
}

void Scene::addMonster(Ogre::Vector3 position)
{
  Monster* monster = this->getWorld()->createMonster();
  monster->setPosition(position);
  monster->setScene(this);
  monsters.push_back(monster);
}

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

  //if(!world->freezeCollisionDebug)

  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) if(*it) (*it)->frameRenderingQueued(evt);//iterate monsters
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) if(*it) (*it)->frameRenderingQueued(evt);//iterate items

  advancePhysics(evt.timeSinceLastFrame);
}

bool Scene::advancePhysics(Ogre::Real dt)
{
  accumulator += dt;
  if(accumulator < stepSize) return false;
  accumulator -= stepSize;
  physicsManager->simulate(stepSize);

  //profiling box creation
  if(addItems) addItem(Ogre::Vector3(genXOrigin + genRadius * std::cos(genAngle),300,genYOrigin + genRadius * std::sin(genAngle)));
  genAngle += 0.1f;


  return true;
}

void Scene::generateCave()
{

}

void Scene::generateDungeon()
{
  Ogre::Real SCALE(100.0f);
  Ogre::String CORNER = world->getDataManager()->getArchitecture(75)->mesh;
  Ogre::String CENTRE = world->getDataManager()->getArchitecture(71)->mesh;
  Ogre::String EDGE = world->getDataManager()->getArchitecture(95)->mesh;
  Ogre::String EDGE_ENTRANCE = world->getDataManager()->getArchitecture(99)->mesh;

  Generator::Dungeon dungeon = Generator::Dungeon(30, 30, 15);
  for(std::vector<Generator::Room*>::iterator it = dungeon.rooms.begin(); it < dungeon.rooms.end(); ++it)
  {
    Generator::Room* room = (*it);

    for(int i = room->topLeft.x + 1; i < room->topRight.x; i++)
    {
      for(int j = room->topLeft.y + 1; j < room->bottomLeft.y; j++)
      {
        architecture->add(CENTRE, Ogre::Vector3(i * SCALE, 0, j * SCALE));
      }
    }

    Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;

    Ogre::Real i = 0.0f;
    for(std::vector<Generator::Point*>::iterator cornerIter = room->corners.begin(); cornerIter < room->corners.end(); ++cornerIter)
    {
      rotation.FromAngleAxis(Ogre::Degree(Ogre::Real(i)), Ogre::Vector3::UNIT_Y);
      architecture->add(CORNER, Ogre::Vector3((*cornerIter)->x * SCALE, 0, (*cornerIter)->y * SCALE), rotation);
      i += 90.0f;
    }

    for(int j = room->topLeft.x + 1; j < room->topRight.x; j++)
    {
      rotation.FromAngleAxis(Ogre::Degree(90), Ogre::Vector3::UNIT_Y);
      if(room->isEntrance(Generator::Point(j,room->topLeft.y - 1))) architecture->add(EDGE_ENTRANCE, Ogre::Vector3(j * SCALE, 0, room->topLeft.y * SCALE), rotation);
      else architecture->add(EDGE, Ogre::Vector3(j * SCALE, 0, room->topLeft.y * SCALE), rotation);

      rotation.FromAngleAxis(Ogre::Degree(270), Ogre::Vector3::UNIT_Y);
      if(room->isEntrance(Generator::Point(j,room->bottomLeft.y + 1))) architecture->add(EDGE_ENTRANCE, Ogre::Vector3(j * SCALE, 0, room->bottomLeft.y * SCALE), rotation);
      architecture->add(EDGE, Ogre::Vector3(j * SCALE, 0, room->bottomLeft.y * SCALE), rotation);
    }

    for(int j = room->topLeft.y + 1; j < room->bottomLeft.y; j++)
    {
      rotation.FromAngleAxis(Ogre::Degree(180), Ogre::Vector3::UNIT_Y);
      if(room->isEntrance(Generator::Point(room->topLeft.x - 1, j))) architecture->add(EDGE_ENTRANCE, Ogre::Vector3(room->topLeft.x * SCALE, 0, j * SCALE), rotation);
      else architecture->add(EDGE, Ogre::Vector3(room->topLeft.x * SCALE, 0, j * SCALE), rotation);

      rotation.FromAngleAxis(Ogre::Degree(0), Ogre::Vector3::UNIT_Y);
      if(room->isEntrance(Generator::Point(room->topRight.x + 1, j))) architecture->add(EDGE_ENTRANCE, Ogre::Vector3(room->topRight.x * SCALE, 0, j * SCALE), rotation);
      else architecture->add(EDGE, Ogre::Vector3(room->topRight.x * SCALE, 0, j * SCALE), rotation);
    }
  }

  //dungeon.output();
}

void Scene::generateTown()
{

}

void Scene::generatePredefined()
{
  for(int i = 19; i <= 32; i++) architecture->add(world->getDataManager()->getArchitecture(i)->mesh);
  architecture->add(world->getDataManager()->getArchitecture(33)->mesh, Ogre::Vector3(-380.0f, 0.0f, 0.0f));

  for(int i = 0; i < 10; i++)
  {
    architecture->add(world->getDataManager()->getArchitecture(47)->mesh, Ogre::Vector3(-445.0f - 100.0f * i, 0.0f, 0.0f));
  }
}

void Scene::loadCharLevel(Ogre::String file)
{
  name = "test";

  std::string line;
	std::ifstream infile;
	infile.open(file);
  unsigned int row = 0;
  while(std::getline(infile, line))
  {
    for(unsigned int col = 0; col < line.length(); col++)
    {
      if(line[col] == '.') continue;

      Ogre::Vector3 position(row * 100.0f, 0.0f, col * 100.0f);
      if(line[col] == '!') 
      {
        architecture->add(world->getDataManager()->getArchitecture(71)->mesh, position);
        addLight(Ogre::Vector3(row * 100.0f + 50.0f, 50.0f, col * 100.0f + 50.0f),true,4000.0f);
      }
      
      Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
      int remainder = ((int) line[col] - 47) % 4;//getting required rotation
      rotation.FromAngleAxis(Ogre::Degree(remainder * 90.0f), Ogre::Vector3::UNIT_Y);

      ArchitectureModel* model = world->getDataManager()->getArchitecture(((int)line[col]) - remainder);
      if(model) architecture->add(model->mesh, position, rotation);
    }
    row++;
  }
	infile.close();
}

void Scene::loadXmlLevel(Ogre::String file)
{



}

World* Scene::getWorld()
{
  return world;
}





