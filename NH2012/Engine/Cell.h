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
#include "Environment.h"

#include "../Generator/Dungeon.h"
#include "../Generator/Room.h"
#include "../Generator/Entrance.h"
#include "../Generator/Point.h"


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

namespace CellType
{
  enum Type
  {
    OVERWORLD,
    UNDERWORLD,
    CAVE,
    DUNGEON,
    TOWN,
    PREDEFINED,
    ASTRAL,
    FILE
  };
}

class Player;
class Monster;//forward declaring for circular dependency

class Cell
{
public:
  Cell(Ogre::Root* root, Environment* environment, 
       Ogre::String name = Ogre::String("Default Dungeon"), 
       CellType::Type type = CellType::PREDEFINED);
  ~Cell(void);

  Ogre::String getName();

  void addPlayer(Player* player);
  void addMonster(Ogre::Vector3 position);
  void addItem(Ogre::Vector3 position);
  void addLight(Ogre::Vector3 position, bool castShadows, Ogre::Real range);
  void addParticles(Ogre::String name, Ogre::Vector3 position, Ogre::Vector3 scale, Ogre::Real speed);

  void removePlayer(Player* player);

  Ogre::SceneManager* getSceneManager();
  OgreBulletDynamics::DynamicsWorld* getPhysicsWorld();

  void frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
  Environment* environment;

  Ogre::SceneManager* sceneManager;
  OgreBulletDynamics::DynamicsWorld* physics;

  Ogre::String name;
  CellType::Type type;

  Architecture* architecture;
  std::vector<Ogre::Light*> lights;
  std::vector<Ogre::ParticleSystem*> particles;
  std::vector<Monster*> monsters;
  std::vector<Item*> items;
  Player* player;

  int instanceNumber;

  void generateOverworld();
  void generateUnderworld();
  void generateCave();
  void generateDungeon();
  void generateTown();
  void generatePredefined();
  void generateAstral();

  void load(Ogre::String file);
};

