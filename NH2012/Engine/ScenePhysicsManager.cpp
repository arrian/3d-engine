#include "ScenePhysicsManager.h"

#include "NHException.h"


ScenePhysicsManager::ScenePhysicsManager(PhysicsManager* physicsManager, Vector3 gravity)
  : physicsManager(physicsManager),
    numberPhysicsCPUThreads(4),
    stepSize(1.0 / 60.0),
    accumulator(0.0),
    sceneControllerManager(NULL),
    scenePhysics(NULL)
{
  //static physx::PxDefaultSimulationFilterShader defaultFilterShader;//??

  //scene physics
  physx::PxSceneDesc physicsDesc(physicsManager->getTolerancesScale());
  physicsDesc.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);

  physicsDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(numberPhysicsCPUThreads);
  if(!physicsDesc.cpuDispatcher) throw NHException("could not create scene CPU dispatcher");

  physicsDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
  if(!physicsDesc.filterShader) throw NHException("filter shader creation failed");

  scenePhysics = physicsManager->getPhysics()->createScene(physicsDesc);
  if(!scenePhysics) throw NHException("could not create scene physics manager");

  sceneControllerManager = PxCreateControllerManager(*physicsManager->getFoundation());
  if(!sceneControllerManager) throw NHException("could not create scene controller manager");

#ifdef _DEBUG
  scenePhysics->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
  scenePhysics->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
#endif
}

//-------------------------------------------------------------------------------------
ScenePhysicsManager::~ScenePhysicsManager(void)
{
  scenePhysics->release();
}

//-------------------------------------------------------------------------------------
bool ScenePhysicsManager::update(double elapsedSeconds)
{
  if(!physicsManager->isEnabled()) return true;//only progress scene if physics is enabled (maybe move into scenePhysicsManager)

  accumulator += elapsedSeconds;
  if(accumulator < stepSize) return false;
  accumulator -= stepSize;
  scenePhysics->simulate((float) stepSize);
  scenePhysics->fetchResults(true);

  return true;
}

//-------------------------------------------------------------------------------------
void ScenePhysicsManager::setGravity(Vector3 gravity)
{
  scenePhysics->setGravity(physx::PxVec3(gravity.x, gravity.y, gravity.z));
}

//-------------------------------------------------------------------------------------
Vector3 ScenePhysicsManager::getGravity()
{
  physx::PxVec3 gravity = scenePhysics->getGravity();
  return Vector3(gravity.x, gravity.y, gravity.z);
}



