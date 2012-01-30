#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "Monster.h"
#include "Item.h"

namespace DungeonType
{
  enum Type
  {
    OVERWORLD,
    UNDERWORLD,
    CAVE,
    DUNGEON,
    TOWN,
    PREDEFINED,
    ASTRAL
  };
}

class Monster;//forward declaring for circular dependency

class Dungeon
{
public:
  Dungeon(Ogre::SceneManager* sceneManager, Ogre::String name = Ogre::String("Default Dungeon"), DungeonType::Type type = DungeonType::PREDEFINED, int numMonsters = 0, int numItems = 0, Ogre::ColourValue colour = Ogre::ColourValue(1,1,1));
  ~Dungeon(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
  Ogre::SceneManager* sceneManager;

  Ogre::String name;
  DungeonType::Type type;

  Ogre::ColourValue colour;

  std::vector<Monster> monsters;
  std::vector<Item> items;
};

