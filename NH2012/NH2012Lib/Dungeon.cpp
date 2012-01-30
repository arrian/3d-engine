#include "Dungeon.h"

Dungeon::Dungeon(Ogre::SceneManager* sceneManager, Ogre::String name, DungeonType::Type type, int numMonsters, int numItems, Ogre::ColourValue colour)
{
  this->sceneManager = sceneManager;

  this->name = name;
  this->type = type;
  this->colour = colour;

  monsters = std::vector<Monster>();
  for(int i = 0; i <= numMonsters; i++)
  {
    monsters.push_back(Monster());
  }

  items = std::vector<Item>();
  for(int i = 0; i <= numItems; i++)
  {
    items.push_back(Item());
  }

  segments = std::vector<Segment>();
  if(type == DungeonType::PREDEFINED)
  {
    Ogre::Entity* r1 = sceneManager->createEntity("r1", "r1.mesh");
    Ogre::Entity* r2 = sceneManager->createEntity("r2", "r2.mesh");
    Ogre::Entity* r3 = sceneManager->createEntity("r3", "r3.mesh");
    Ogre::Entity* r4 = sceneManager->createEntity("r4", "r4.mesh");
    Ogre::Entity* r5 = sceneManager->createEntity("r5", "r5.mesh");
    Ogre::Entity* r6 = sceneManager->createEntity("r6", "r6.mesh");
    Ogre::Entity* r7 = sceneManager->createEntity("r7", "r7.mesh");
    Ogre::Entity* r8 = sceneManager->createEntity("r8", "r8.mesh");
    Ogre::Entity* r9 = sceneManager->createEntity("r9", "r9.mesh");
    Ogre::Entity* r10 = sceneManager->createEntity("r10", "r10.mesh");
    Ogre::Entity* r11 = sceneManager->createEntity("r11", "r11.mesh");
    Ogre::Entity* r12 = sceneManager->createEntity("r12", "r12.mesh");
    Ogre::Entity* r13 = sceneManager->createEntity("r13", "r13.mesh");
    Ogre::Entity* r14 = sceneManager->createEntity("r14", "r14.mesh");

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

    // Set ambient light
    sceneManager->setAmbientLight(Ogre::ColourValue(0, 0, 0));

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
}

void Dungeon::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  for(std::vector<Monster>::iterator it = monsters.begin(); it != monsters.end(); ++it) 
  {
    it->frameRenderingQueued(evt);
  }
}