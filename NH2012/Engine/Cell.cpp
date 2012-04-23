#include "Cell.h"

Cell::Cell(World* world, Ogre::String name, SceneType type)
  : world(world),
    sceneManager(world->getRoot()->createSceneManager(Ogre::ST_INTERIOR)),
    instanceNumber(0),
    physics(new OgreBulletDynamics::DynamicsWorld(sceneManager, Ogre::AxisAlignedBox(Ogre::Vector3(-10000.0f,-10000.0f,-10000.0f), Ogre::Vector3(10000.0f,10000.0f,10000.0f)), Ogre::Vector3(0.0f,-9.807f,0.0f))),
    name(name),
    type(type),
    monsters(std::vector<Monster*>()),
    items(std::vector<Item*>()),
    architecture(new Architecture(sceneManager, physics)),
    lights(std::vector<Ogre::Light*>()),
    particles(std::vector<Ogre::ParticleSystem*>()),
    player(0),
    active(false),
    physicsManager(0)
{
  //scene physics
  physx::PxSceneDesc desc(world->getTolerancesScale());
  desc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
  physicsManager = world->getPhysics()->createScene(desc);
  physics->setShowDebugShapes(world->showCollisionDebug);

  if(world->enableShadows) sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

  sceneManager->setDisplaySceneNodes(world->isDebug());
  sceneManager->setShowDebugShadows(world->isDebug());
  sceneManager->showBoundingBoxes(world->isDebug());
  
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
}

Cell::~Cell(void)
{

  if(architecture) delete architecture;
  architecture = 0;

  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it)  
  {
    if(*it) delete (*it);
    (*it) = 0;
  }

  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it)
  {
    if(*it) delete (*it);
    (*it) = 0;
  }

  if(physics) delete physics;
  physics = 0;

  world->getRoot()->destroySceneManager(sceneManager);
}

bool Cell::isActive()
{
  return active;
}

Ogre::String Cell::getName()
{
  return name;
}

Ogre::SceneManager* Cell::getSceneManager()
{
  return sceneManager;
}

OgreBulletDynamics::DynamicsWorld* Cell::getPhysicsWorld()
{
  return physics;
}

physx::PxScene* Cell::getPhysicsManager()
{
  return physicsManager;
}

void Cell::addPlayer(Player* player)
{
  active = true;
  player->setCell(this, Ogre::Vector3(800,50,200), Ogre::Vector3(800,50,600));
  this->player = player;
}

void Cell::addMonster(Ogre::Vector3 position)
{
  monsters.push_back(new Monster(sceneManager, physics, position, instanceNumber));
  instanceNumber++;
}

void Cell::addItem(Ogre::Vector3 position)
{
  items.push_back(new Item(sceneManager, physics, position, instanceNumber));
  instanceNumber++;
}

void Cell::addLight(Ogre::Vector3 position, bool castShadows, Ogre::Real range)
{
  Ogre::Light* light = sceneManager->createLight("light" + Ogre::StringConverter::toString(instanceNumber));
  lights.push_back(light);
  light->setPosition(position);
  light->setAttenuation(range, 1.0f, 0.0014f, 0.000007f);
  light->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 0.95f));
  light->setCastShadows(castShadows);
  instanceNumber++;
}

void Cell::addParticles(Ogre::String name, Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::Real speed)
{
  Ogre::ParticleSystem* particle = sceneManager->createParticleSystem(name);//"Rain");//, "Examples/Rain");
  particle->fastForward(speed);
  Ogre::SceneNode* particleNode = sceneManager->getRootSceneNode()->createChildSceneNode("particle" + Ogre::StringConverter::toString(instanceNumber));
  particleNode->setPosition(position);
  particleNode->setScale(scale);
  particleNode->attachObject(particle);
  particles.push_back(particle);
  instanceNumber++;
}

void Cell::removePlayer(Player* player)
{
  if(this->player == player) 
  {
    this->player = 0;
    active = false;//when more than one player, will need to check all players before deactivating cell
  }
}

void Cell::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!world->freezeCollisionDebug) physics->stepSimulation(evt.timeSinceLastFrame);

  if(player) player->frameRenderingQueued(evt);

  if(!world->freezeCollisionDebug) 
  {
    for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) (*it)->frameRenderingQueued(evt);

    physics->stepSimulation(evt.timeSinceLastFrame);
  }
}

void Cell::generateCave()
{

}

void Cell::generateDungeon()
{
  int SCALE = 100;
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

    int i = 0;
    for(std::vector<Generator::Point*>::iterator cornerIter = room->corners.begin(); cornerIter < room->corners.end(); ++cornerIter)
    {
      rotation.FromAngleAxis(Ogre::Degree(i), Ogre::Vector3::UNIT_Y);
      architecture->add(CORNER, Ogre::Vector3((*cornerIter)->x * SCALE, 0, (*cornerIter)->y * SCALE), rotation);
      i += 90;
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

void Cell::generateTown()
{

}

void Cell::generatePredefined()
{
  for(int i = 19; i <= 32; i++) architecture->add(world->getDataManager()->getArchitecture(i)->mesh);
  architecture->add(world->getDataManager()->getArchitecture(33)->mesh, Ogre::Vector3(-380,0,0));

  for(int i = 0; i < 10; i++)
  {
    architecture->add(world->getDataManager()->getArchitecture(47)->mesh, Ogre::Vector3(-445 - 100 * i,0,0));
  }
}

void Cell::loadCharLevel(Ogre::String file)
{
  name = "test";

  std::string line;
	std::ifstream infile;
	infile.open(file);
  int row = 0;
  while(std::getline(infile, line))
  {
    for(int col = 0; col < line.length(); col++)
    {
      if(line[col] == '.') continue;

      Ogre::Vector3 position(row * 100, 0, col * 100);
      if(line[col] == '!') 
      {
        architecture->add(world->getDataManager()->getArchitecture(71)->mesh, position);
        addLight(Ogre::Vector3(row * 100 + 50, 50, col * 100 + 50),true,4000);
      }
      
      Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
      int remainder = ((int) line[col] - 47) % 4;//getting required rotation
      rotation.FromAngleAxis(Ogre::Degree(remainder * 90), Ogre::Vector3::UNIT_Y);

      ArchitectureModel* model = world->getDataManager()->getArchitecture(((int)line[col]) - remainder);
      if(model) architecture->add(model->mesh, position, rotation);
    }
    row++;
  }
	infile.close();
}

void Cell::loadXmlLevel(Ogre::String file)
{



}



