#pragma once

#include <OgreFrameListener.h>
#include <OgreString.h>
#include <OgreVector3.h>

#include "Actor.h"
#include "MonsterData.h"
#include "Dungeon.h"
#include "Inventory.h"
#include "Attributes.h"
#include "Bar.h"

class Dungeon;//forward declaring for circular dependency

class Monster : public Actor, public MonsterData
{
public:
  Monster(Ogre::SceneManager* sceneManager, OgreBulletDynamics::DynamicsWorld* physics, 
          Ogre::Vector3 position, int id = 0, int difficulty = 1);
  virtual ~Monster(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  /*! Sets the monsters target position.*/
  void setTarget(Ogre::Vector3 target);
private:
  Ogre::Vector3 target;

  void animation();
  void audio();
  void collision();

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

