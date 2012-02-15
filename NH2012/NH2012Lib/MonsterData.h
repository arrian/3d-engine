#pragma once

#include <OgreString.h>

#include "Attributes.h"
#include "Bar.h"
#include "Inventory.h"

class MonsterData
{
public:
  MonsterData(void);
  virtual ~MonsterData(void);

  friend bool operator==(const MonsterData& x, const MonsterData& y);
protected:
  /*! Monster name.*/
  Ogre::String name;

  std::vector<MonsterAttribute::Ability> abilities;
  MonsterAttribute::Attack attack;
  MonsterAttribute::Awareness awareness;
  MonsterAttribute::Behaviour behaviour;
  std::vector<MonsterAttribute::Body> body;
  std::vector<MonsterAttribute::Type> types;
  std::vector<MonsterAttribute::LikesDislikes> dislikes;
  MonsterAttribute::Gender gender;
  std::vector<MonsterAttribute::LikesDislikes> likes;
  MonsterAttribute::Size size;
  std::vector<MonsterAttribute::Produces> produces;
  std::vector<GeneralAttribute::Intrinsics> intrinsics;
  MonsterAttribute::Status status;

  /*! Number of units to move per second ~100 = 1 metre per second.*/
  Ogre::Real speed;

  /*! Height of the actor in units.*/
  Ogre::Real height;

  Bar level;
  Bar health;
  Bar magic;

  Ogre::Real magicResistance;
  Ogre::Real meleeResistance;

  Inventory inventory;
};

