#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include <OgreVector3.h>

#include "BasicComponent.h"
#include "HumanoidSkeletonComponent.h"
#include "IntelligenceComponent.h"
#include "VisualComponent.h"
#include "Attributes.h"
#include "Bar.h"

class Scene;

class Monster : public BasicComponent
{
public:
  Monster(int id);
  virtual ~Monster(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void setPosition(Ogre::Vector3 position);

  /*! Sets the monsters target position.*/
  void setTarget(Ogre::Vector3 target);

  friend bool operator==(const Monster& x, const Monster& y);

protected:
  int id;

  IntelligenceComponent intelligence;
  HumanoidSkeletonComponent skeleton;
  VisualComponent visual;

  Ogre::SceneNode* node;

  Ogre::Vector3 position;
  Ogre::Vector3 target;

  /*
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
  */

  /*! Number of units to move per second ~100 = 1 metre per second.*/
  Ogre::Real speed;

  /*! Height of the actor in units.*/
  Ogre::Real height;

  /*
  Bar level;
  Bar health;
  Bar magic;

  Ogre::Real magicResistance;
  Ogre::Real meleeResistance;
  */

  void hasSceneChange();
};

