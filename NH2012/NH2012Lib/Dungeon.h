#pragma once

#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreColourValue.h>
#include <OgreEntity.h>
#include <OgreParticleSystem.h>

#include "Player.h."
#include "Monster.h"
#include "Item.h"
#include "Architecture.h"
#include "Flag.h"


#include "OgreBulletCollisionsShape.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsConeShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "Shapes/OgreBulletCollisionsTriangleShape.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsCompoundShape.h"
#include "Shapes/OgreBulletCollisionsMultiSphereShape.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsMinkowskiSumShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "OgreBulletCollisionsRay.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"
#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsConstraint.h"
#include "Constraints/OgreBulletDynamicsPoint2pointConstraint.h" 

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

class Player;
class Monster;//forward declaring for circular dependency

class Dungeon
{
public:
  Dungeon(Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window, 
          Flag* flags, Ogre::String name = Ogre::String("Default Dungeon"), 
          DungeonType::Type type = DungeonType::PREDEFINED, int numMonsters = 0, 
          int numItems = 0, Ogre::ColourValue colour = Ogre::ColourValue(1,1,1));
  ~Dungeon(void);

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

  void injectKeyDown(const OIS::KeyEvent &arg);
  void injectKeyUp(const OIS::KeyEvent &arg);
  void injectMouseMove(const OIS::MouseEvent &arg);
  void injectMouseDown(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  void injectMouseUp(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
private:
  Flag* flags;

  Ogre::SceneManager* sceneManager;
  OgreBulletDynamics::DynamicsWorld* physics;

  Ogre::String name;
  DungeonType::Type type;

  Ogre::ColourValue colour;

  Architecture* architecture;
  std::vector<Ogre::Light*> lights;
  std::vector<Monster*> monsters;
  std::vector<Item*> items;
  Player* player;

  int instanceNumber;
};

