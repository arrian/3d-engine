#pragma once

#include "PhysicsManager.h"
#include "Vector3.h"

#include "PxPhysicsAPI.h"
#include "characterkinematic/PxControllerManager.h"
#include "extensions/PxDefaultCpuDispatcher.h"
#include "extensions/PxDefaultSimulationFilterShader.h"
#include "extensions/PxSimpleFactory.h"
#include "PxVisualizationParameter.h"

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

class Scene;

class ScenePhysicsManager
{
public:
  ScenePhysicsManager(boost::shared_ptr<Scene> scene, Vector3 gravity);
  virtual ~ScenePhysicsManager(void);

  void setGravity(Vector3 gravity);

  bool update(double elapsedSeconds);

  Vector3 getGravity();
  physx::PxScene* getScenePhysics() {return scenePhysics;}
  physx::PxControllerManager* getSceneControllerManager() {return sceneControllerManager;}

  void addActor(physx::PxActor& actor) {scenePhysics->addActor(actor);}

  boost::shared_ptr<Scene> getScene() 
  {
    try
    {
      return scene.lock();
    }
    catch(boost::bad_weak_ptr b)
    {
#ifdef _DEBUG
      std::cout << "Could not get scene from scene physics manager. Scene has expired." << std::endl;
#endif
    }
    return boost::shared_ptr<Scene>();
  }

private:
  //PhysicsManager* physicsManager;
  boost::weak_ptr<Scene> scene;

  physx::PxScene* scenePhysics;
  physx::PxControllerManager* sceneControllerManager;//may only need one manager in the world

  double accumulator;
  double stepSize;
  physx::PxU32 numberPhysicsCPUThreads;
  
};

