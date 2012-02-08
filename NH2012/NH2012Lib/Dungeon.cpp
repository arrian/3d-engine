#include "Dungeon.h"

Dungeon::Dungeon(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, 
                 Flag* flags, Ogre::String name, DungeonType::Type type, 
                 int numMonsters, int numItems, Ogre::ColourValue colour)
{
  this->sceneManager = sceneManager;
  this->flags = flags;

  instanceNumber = 0;
  
  physics = new OgreBulletDynamics::DynamicsWorld(sceneManager, 
                                                  Ogre::AxisAlignedBox(Ogre::Vector3(-10000,-10000,-10000),
                                                  Ogre::Vector3(10000,10000,10000)), Ogre::Vector3(0,-9.807,0));
  physics->setShowDebugShapes(flags->showCollisionDebug);

  std::cout << "Creating player" << std::endl;
  player = new Player(sceneManager, physics, flags, window, Ogre::Vector3(0,50,0));
  instanceNumber++;

  this->name = name;
  this->type = type;
  this->colour = colour;

  std::cout << "Creating monsters" << std::endl;
  monsters = std::vector<Monster*>();
  for(int i = 1; i <= numMonsters; i++)
  {
    monsters.push_back(new Monster(sceneManager, physics, Ogre::Vector3(40*i + 40,51,0), instanceNumber));
    instanceNumber++;
  }
  
  std::cout << "Creating items" << std::endl;
  items = std::vector<Item*>();
  for(int i = 1; i <= numItems; i++)
  {
    items.push_back(new Item(sceneManager, physics, Ogre::Vector3(0,10,200*i + 100), instanceNumber));
    instanceNumber++;
  }

  std::cout << "Creating scenery" << std::endl;
  architecture = new Architecture(sceneManager, physics);
  lights = std::vector<Ogre::Light*>();
  if(type == DungeonType::PREDEFINED)
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

    architecture->add("hall.mesh", Ogre::Vector3(-445,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-545,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-645,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-745,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-845,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-945,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-1045,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-1145,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-1245,0,0));
    architecture->add("hall.mesh", Ogre::Vector3(-1345,0,0));
    
    /*
    std::cout << "Creating particles" << std::endl;
    //Particles
    Ogre::ParticleSystem* sunParticle = sceneManager->createParticleSystem("Rain", "Examples/Rain");
    sunParticle->fastForward(20);
    Ogre::SceneNode* particleNode = sceneManager->getRootSceneNode()->createChildSceneNode("Particle");
    particleNode->setPosition(-100,1000,0);
    particleNode->setScale(0.5,0.5,0.5);
    particleNode->attachObject(sunParticle);
    */

    if(flags->enableLights)
    {
      // Create lights
      std::cout << "Creating lights" << std::endl;

      //Set ambient light
      sceneManager->setAmbientLight(Ogre::ColourValue(0,0,0));

      Ogre::Light* l = sceneManager->createLight("MainLight");
      lights.push_back(l);
      l->setPosition(50,50,50);
      l->setAttenuation(3250, 1.0, 0.0014, 0.000007);
      l->setCastShadows(true);

      Ogre::Light* l2 = sceneManager->createLight("light2");
      lights.push_back(l2);
      l2->setPosition(-600,50,0);
      l2->setAttenuation(3250, 1.0, 0.0014, 0.000007);
      l2->setCastShadows(true);

      Ogre::Light* l3 = sceneManager->createLight("light3");
      lights.push_back(l3);
      l3->setPosition(-800,50,0);
      l3->setAttenuation(3250, 1.0, 0.0014, 0.000007);
      l3->setCastShadows(true);

      Ogre::Light* l4 = sceneManager->createLight("light4");
      lights.push_back(l4);
      l4->setPosition(-1200,50,0);
      l4->setAttenuation(3250, 1.0, 0.0014, 0.000007);
      l4->setCastShadows(true);
    }
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
  if(architecture) delete architecture;
  architecture = 0;

  if(flags->enableLights)
  {
    for(std::vector<Ogre::Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
    {
      if(*it) delete (*it);
      (*it) = 0;
    }
  }

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

void Dungeon::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(!flags->freezeCollisionDebug) physics->stepSimulation(evt.timeSinceLastFrame);

  player->frameRenderingQueued(evt);

  if(!flags->freezeCollisionDebug) 
  {
    for(std::vector<Monster*>::iterator it = monsters.begin(); it != monsters.end(); ++it) 
    {
      (*it)->frameRenderingQueued(evt);
    }

    physics->stepSimulation(evt.timeSinceLastFrame);
  }
}

void Dungeon::injectKeyDown(const OIS::KeyEvent &arg)
{
  if(arg.key == flags->controls.freezeCollision) flags->freezeCollisionDebug = !flags->freezeCollisionDebug;

  if(arg.key == OIS::KC_1) 
  {
    items.push_back(new Item(sceneManager, physics, Ogre::Vector3(0,51,200), instanceNumber));
    instanceNumber++;
  }

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
