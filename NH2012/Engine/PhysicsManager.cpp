#include "PhysicsManager.h"


PhysicsManager::PhysicsManager(void)
  : errorCallback(),
    allocatorCallback()
{
  physicsWorld = NULL;
  physicsMaterial = NULL;
  physicsFoundation = NULL;

  defaultRestitution = 0.2f;
  defaultStaticFriction = 0.3f;
  defaultDynamicFriction = 0.4f;

  //Creating physics
  physx::PxAllocatorCallback* allocator = &allocatorCallback;
  physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, errorCallback);//getErrorCallback());
  physicsFoundation->setErrorLevel(physx::PxErrorCode::eDEBUG_INFO);
  if(!physicsFoundation) throw NHException("physics foundation could not be created");

  bool recordMemoryAllocations = true;
  physicsWorld = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxTolerancesScale(), recordMemoryAllocations);
  if(!physicsWorld) throw NHException("physics world could not be created");

  PxInitExtensions(*physicsWorld);

  physicsCooking = PxCreateCooking(PX_PHYSICS_VERSION, physicsWorld->getFoundation(), physx::PxCookingParams());
  if(!physicsCooking) throw NHException("physics cooker could not be created");

  physicsMaterial = physicsWorld->createMaterial(defaultStaticFriction, defaultDynamicFriction, defaultRestitution);
  if(!physicsMaterial) throw NHException("default physics material could not be created");
}

//-------------------------------------------------------------------------------------
PhysicsManager::~PhysicsManager(void)
{
  physicsWorld->release();
  physicsWorld = NULL;

  physicsFoundation->release();
  physicsFoundation = NULL;
}

//-------------------------------------------------------------------------------------
void PhysicsManager::initialiseVisualDebugger(std::string ip, int port, int timeoutMilliseconds)
{
  physx::PxExtensionVisualDebugger::connect(physicsWorld->getPvdConnectionManager(), ip.c_str(), port, timeoutMilliseconds);
  physicsWorld->getVisualDebugger()->setVisualizeConstraints(true);
}

//-------------------------------------------------------------------------------------
physx::PxCooking* PhysicsManager::getCooking()
{
  return physicsCooking;
}

//-------------------------------------------------------------------------------------
physx::PxPhysics* PhysicsManager::getPhysics()
{
  return physicsWorld;
}

//-------------------------------------------------------------------------------------
physx::PxMaterial* PhysicsManager::getDefaultMaterial()
{
  return physicsMaterial;
}

//-------------------------------------------------------------------------------------
physx::PxFoundation* PhysicsManager::getFoundation()
{
  return physicsFoundation;
}

//-------------------------------------------------------------------------------------
const physx::PxTolerancesScale& PhysicsManager::getTolerancesScale()
{
  return physicsWorld->getTolerancesScale();
}