#include "Dungeon.h"

Dungeon::Dungeon(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, Ogre::String name, DungeonType::Type type, int numMonsters, int numItems, Ogre::ColourValue colour)
{
  this->sceneManager = sceneManager;

  debugPause = true;
  
  physics = new OgreBulletDynamics::DynamicsWorld(sceneManager, Ogre::AxisAlignedBox(Ogre::Vector3(-10000,-10000,-10000),Ogre::Vector3(10000,10000,10000)), Ogre::Vector3(0,-9.807,0));
  physics->setShowDebugShapes(true);

  std::cout << "Creating player" << std::endl;
  player = new Player(sceneManager, physics, window, Ogre::Vector3(0,51,0));

  this->name = name;
  this->type = type;
  this->colour = colour;

  std::cout << "Creating monsters" << std::endl;
  monsters = std::vector<Monster*>();
  for(int i = 1; i <= numMonsters; i++)
  {
    monsters.push_back(new Monster(sceneManager, physics, Ogre::Vector3(40*i + 40,51,0), i));
  }
  
  std::cout << "Creating items" << std::endl;
  items = std::vector<Item*>();
  for(int i = 1; i <= numItems; i++)
  {
    items.push_back(new Item(sceneManager, physics, Ogre::Vector3(0,10,200*i + 100), i + numMonsters + 1));
  }

  std::cout << "Creating scenery" << std::endl;
  if(type == DungeonType::PREDEFINED)
  {
    Ogre::Entity* r1 = sceneManager->createEntity("r1.mesh");
    Ogre::Entity* r2 = sceneManager->createEntity("r2.mesh");
    Ogre::Entity* r3 = sceneManager->createEntity("r3.mesh");
    Ogre::Entity* r4 = sceneManager->createEntity("r4.mesh");
    Ogre::Entity* r5 = sceneManager->createEntity("r5.mesh");
    Ogre::Entity* r6 = sceneManager->createEntity("r6.mesh");
    Ogre::Entity* r7 = sceneManager->createEntity("r7.mesh");
    Ogre::Entity* r8 = sceneManager->createEntity("r8.mesh");
    Ogre::Entity* r9 = sceneManager->createEntity("r9.mesh");
    Ogre::Entity* r10 = sceneManager->createEntity("r10.mesh");
    Ogre::Entity* r11 = sceneManager->createEntity("r11.mesh");
    Ogre::Entity* r12 = sceneManager->createEntity("r12.mesh");
    Ogre::Entity* r13 = sceneManager->createEntity("r13.mesh");
    Ogre::Entity* r14 = sceneManager->createEntity("r14.mesh");

    Ogre::SceneNode* testRoom = sceneManager->getRootSceneNode()->createChildSceneNode();

    testRoom->attachObject(r1);
    testRoom->attachObject(r2);
    testRoom->attachObject(r3);
    testRoom->attachObject(r4);
    testRoom->attachObject(r5);
    testRoom->attachObject(r6);
    testRoom->attachObject(r7);
    testRoom->attachObject(r8);
    testRoom->attachObject(r9);
    testRoom->attachObject(r10);
    testRoom->attachObject(r11);
    testRoom->attachObject(r12);
    testRoom->attachObject(r13);
    testRoom->attachObject(r14);

    addStaticTrimesh(r6,testRoom,0.5,0.5);


    //Set ambient light
    sceneManager->setAmbientLight(Ogre::ColourValue(0,0,0));

    /*
    //Particles
    Ogre::ParticleSystem* sunParticle = sceneManager->createParticleSystem("Rain", "Examples/Rain");
    sunParticle->fastForward(20);
    Ogre::SceneNode* particleNode = sceneManager->getRootSceneNode()->createChildSceneNode("Particle");
    particleNode->setPosition(-100,1000,0);
    particleNode->setScale(0.5,0.5,0.5);
    particleNode->attachObject(sunParticle);
    */

    // Create a light
    Ogre::Light* l = sceneManager->createLight("MainLight");
    l->setPosition(50,50,50);
    l->setAttenuation(3250, 1.0, 0.0014, 0.000007);
    l->setCastShadows(true);
  }
  else if(type == DungeonType::CAVE)
  {
    //generate perlin noise cave
  }
  else if(type == DungeonType::OVERWORLD)
  {
    //generate perlin noise landscape
  }
  else if(type == DungeonType::DUNGEON)
  {
    //generate nethack style dungeon
  }
  else
  {
    //generate other...
  }
}

Dungeon::~Dungeon(void)
{
  if(player) delete player;
  player = 0;
  for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) if(*it) 
  {
    delete (*it);
    (*it) = 0;
  }
  for(std::vector<Item*>::iterator it = items.begin(); it != items.end(); ++it) if(*it) 
  {
    delete (*it);
    (*it) = 0;
  }
  if(physics) delete physics;
  physics = 0;
}

void Dungeon::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!debugPause) physics->stepSimulation(evt.timeSinceLastFrame);

  player->frameRenderingQueued(evt);

  if(!debugPause) 
  {
    for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) 
    {
      (*it)->frameRenderingQueued(evt);
    }

    physics->stepSimulation(evt.timeSinceLastFrame);
  }
}

OgreBulletDynamics::RigidBody* Dungeon::addStaticTrimesh(Ogre::Entity* entity, Ogre::SceneNode* node, Ogre::Real restitution, const Ogre::Real friction)
{
  OgreBulletCollisions::StaticMeshToShapeConverter* trimeshConverter = new OgreBulletCollisions::StaticMeshToShapeConverter(entity);
  OgreBulletCollisions::TriangleMeshCollisionShape* sceneTriMeshShape = trimeshConverter->createTrimesh();
  delete trimeshConverter;
  OgreBulletDynamics::RigidBody* sceneRigid = new OgreBulletDynamics::RigidBody(
      "test Rigid" + Ogre::StringConverter::toString(1),
      physics);
  sceneRigid->setStaticShape(node, sceneTriMeshShape, restitution, friction);

  return sceneRigid;
}

void Dungeon::injectKeyDown(const OIS::KeyEvent &arg)
{
  if(arg.key == OIS::KC_Q) debugPause = !debugPause;

  if(arg.key == OIS::KC_Q) debugPause = !debugPause;

  player->injectKeyDown(arg);
}

void Dungeon::injectKeyUp(const OIS::KeyEvent &arg)
{
  player->injectKeyUp(arg);
}

void Dungeon::injectMouseMove(const OIS::MouseEvent &arg)
{
  player->injectMouseMove(arg);
}

void Dungeon::injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseDown(arg,id);
}

void Dungeon::injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
  player->injectMouseUp(arg,id);
}
