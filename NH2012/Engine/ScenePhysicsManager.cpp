#include "ScenePhysicsManager.h"

#include "NHException.h"
#include "Scene.h"
#include "World.h"


ScenePhysicsManager::ScenePhysicsManager(boost::shared_ptr<Scene> scene, Vector3 gravity)
  : //physicsManager(physicsManager),
    scene(boost::weak_ptr<Scene>(scene)),
    numberPhysicsCPUThreads(4),
    stepSize(1.0 / 60.0),
    accumulator(0.0),
    sceneControllerManager(NULL),
    scenePhysics(NULL)
{
  //static physx::PxDefaultSimulationFilterShader defaultFilterShader;//??

  //TODO: acquire world shared pointer here

  //scene physics
  physx::PxSceneDesc physicsDesc(scene->getWorld()->getPhysicsManager()->getTolerancesScale());
  physicsDesc.gravity = physx::PxVec3(gravity.x, gravity.y, gravity.z);
  physicsDesc.num16KContactDataBlocks = 1024;//testing... default is 256

  physicsDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(numberPhysicsCPUThreads);
  if(!physicsDesc.cpuDispatcher) throw NHException("could not create scene CPU dispatcher");

  physicsDesc.filterShader = &physx::PxDefaultSimulationFilterShader;
  if(!physicsDesc.filterShader) throw NHException("filter shader creation failed");

  scenePhysics = scene->getWorld()->getPhysicsManager()->getPhysics()->createScene(physicsDesc);
  if(!scenePhysics) throw NHException("could not create scene physics manager");

  sceneControllerManager = PxCreateControllerManager(*(scene->getWorld()->getPhysicsManager()->getFoundation()));
  if(!sceneControllerManager) throw NHException("could not create scene controller manager");

#ifdef _DEBUG
  scenePhysics->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
  scenePhysics->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
#endif
}

//-------------------------------------------------------------------------------------
ScenePhysicsManager::~ScenePhysicsManager(void)
{
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(!scene_ptr)
  {
#ifdef _DEBUG
    std::cout << "Scene expired before calling scene physics manager destructor." << std::endl;
#endif
    return;
  }

  //TODO: check if world physics is still alive
  sceneControllerManager->release();
  scenePhysics->release();
}

//-------------------------------------------------------------------------------------
bool ScenePhysicsManager::update(double elapsedSeconds)
{
  boost::shared_ptr<Scene> scene_ptr = getScene();
  if(!scene_ptr)
  {
    #ifdef _DEBUG
    std::cout << "Scene expired before calling scene physics manager update." << std::endl;
    #endif
    return true;
  }

  boost::shared_ptr<World> world_ptr = scene_ptr->getWorld();
  if(!world_ptr)
  {
    #ifdef _DEBUG
    std::cout << "World expired before calling scene physics manager update." << std::endl;
    #endif
    return true;
  }

  if(!world_ptr->getPhysicsManager()->isEnabled()) return true;//only progress scene if physics is enabled (maybe move into scenePhysicsManager)

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



