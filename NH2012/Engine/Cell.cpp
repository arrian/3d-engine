#include "Cell.h"

Cell::Cell(Ogre::Root* root, Environment* environment, Ogre::String name, CellType::Type type)
  : sceneManager(root->createSceneManager(Ogre::ST_INTERIOR)),
    environment(environment),
    instanceNumber(0),
    physics(new OgreBulletDynamics::DynamicsWorld(sceneManager, Ogre::AxisAlignedBox(Ogre::Vector3(-10000,-10000,-10000), Ogre::Vector3(10000,10000,10000)), Ogre::Vector3(0,-9.807,0))),
    name(name),
    type(type),
    monsters(std::vector<Monster*>()),
    items(std::vector<Item*>()),
    architecture(new Architecture(sceneManager, physics)),
    lights(std::vector<Ogre::Light*>()),
    particles(std::vector<Ogre::ParticleSystem*>()),
    player(0)
{
  
  physics->setShowDebugShapes(environment->showCollisionDebug);
  
  switch(type)
  {
  case CellType::PREDEFINED: generatePredefined(); break;
  case CellType::CAVE: generateCave(); break;
  case CellType::OVERWORLD: generateOverworld(); break;
  case CellType::DUNGEON: generateDungeon(); break;
  case CellType::TOWN: generateTown(); break;
  case CellType::UNDERWORLD: generateUnderworld(); break;
  case CellType::ASTRAL: generateAstral(); break;
  case CellType::FILE: load(name); break;
  default: break;
  }

  if(environment->enableLights) addLight(Ogre::Vector3(50,50,50), true, 3250);
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

void Cell::addPlayer(Player* player)
{
  player->setCell(this, Ogre::Vector3(0,0,0), Ogre::Vector3(0,0,300));
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
  light->setAttenuation(range, 1.0, 0.0014, 0.000007);
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
  if(this->player == player) this->player = 0;
}

void Cell::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!environment->freezeCollisionDebug) physics->stepSimulation(evt.timeSinceLastFrame);

  if(player) player->frameRenderingQueued(evt);

  if(!environment->freezeCollisionDebug) 
  {
    for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) (*it)->frameRenderingQueued(evt);

    physics->stepSimulation(evt.timeSinceLastFrame);
  }
}

void Cell::generateOverworld()
{

}

void Cell::generateUnderworld()
{

}

void Cell::generateCave()
{

}

void Cell::generateDungeon()
{
  int SCALE = 100;
  Ogre::String CORNER = "room_corner.mesh";
  Ogre::String CENTRE = "room_centre.mesh";
  Ogre::String EDGE = "room_edge.mesh";
  Ogre::String EDGE_ENTRANCE = "room_edge_entrance.mesh";

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
  architecture->add("r1.mesh");
  architecture->add("r2.mesh");
  architecture->add("r3.mesh");
  architecture->add("r4.mesh");
  architecture->add("r5.mesh");
  architecture->add("r6.mesh");
  architecture->add("r7.mesh");
  architecture->add("r8.mesh");
  architecture->add("r9.mesh");
  architecture->add("r10.mesh");
  architecture->add("r11.mesh");
  architecture->add("r12.mesh");
  architecture->add("r13.mesh");
  architecture->add("r14.mesh");
  architecture->add("connector.mesh", Ogre::Vector3(-380,0,0));

  for(int i = 0; i < 10; i++)
  {
    architecture->add("hall.mesh", Ogre::Vector3(-445 - 100 * i,0,0));
  }
}

void Cell::generateAstral()
{

}

void Cell::load(Ogre::String file)
{

}
