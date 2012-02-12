#pragma once

#include <OgreString.h>

#include "Attributes.h"

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
};

