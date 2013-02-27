#pragma once

#include "PhysicsManager.h"
#include "Vector3.h"

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSimpleFactory.h"
#include "PxVisualizationParameter.h"

class ScenePhysicsManager
{
public:
  ScenePhysicsManager(PhysicsManager* physicsManager, Vector3 gravity);
  virtual ~ScenePhysicsManager(void);

  void setGravity(Vector3 gravity);

  bool update(double elapsedSeconds);

  Vector3 getGravity();
  physx::PxScene* getScenePhysics() {return scenePhysics;}
  physx::PxControllerManager* getSceneControllerManager() {return sceneControllerManager;}

  void addActor(physx::PxActor& actor) {scenePhysics->addActor(actor);}

private:
  PhysicsManager* physicsManager;

  physx::PxScene* scenePhysics;
  physx::PxControllerManager* sceneControllerManager;//may only need one manager in the world

  double accumulator;
  double stepSize;
  physx::PxU32 numberPhysicsCPUThreads;
  
};

