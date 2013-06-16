#include "PhysicsManager.h"

#include "physxprofilesdk/PxProfileZoneManager.h"

#include "NHException.h"

PhysicsManager::PhysicsManager(void)
  : errorCallback(),
    allocatorCallback(),
    fabrication(),
    physicsWorld(NULL),
    physicsCooking(NULL),
    physicsMaterial(NULL),
    physicsFoundation(NULL),
    defaultRestitution(0.2f),
    defaultStaticFriction(0.3f),
    defaultDynamicFriction(0.4f)
{
  //Creating physics
  physx::PxAllocatorCallback* allocator = &allocatorCallback;
  physicsFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *allocator, errorCallback);//getErrorCallback());
  if(!physicsFoundation) throw NHException("physics foundation could not be created");

#ifdef _DEBUG //error level
  physicsFoundation->setErrorLevel(physx::PxErrorCode::eMASK_ALL);
#else
  physicsFoundation->setErrorLevel(physx::PxErrorCode::eABORT | physx::PxErrorCode::eINTERNAL_ERROR 
                                  |physx::PxErrorCode::eINVALID_OPERATION | physx::PxErrorCode::eINVALID_PARAMETER 
                                  |physx::PxErrorCode::eOUT_OF_MEMORY);
#endif

#ifdef _DEBUG //profile zone manager
  bool recordMemoryAllocations = true;
  profileZoneManager = &physx::PxProfileZoneManager::createProfileZoneManager(physicsFoundation);
  if(!profileZoneManager) throw NHException("physics profile zone manager could not be created");
#else
  bool recordMemoryAllocations = false;
  profileZoneManager = NULL;
#endif

  physicsWorld = PxCreatePhysics(PX_PHYSICS_VERSION, *physicsFoundation, physx::PxTolerancesScale(), recordMemoryAllocations, profileZoneManager);
  if(!physicsWorld) throw NHException("physics world could not be created");

  PxInitExtensions(*physicsWorld);

  physicsCooking = PxCreateCooking(PX_PHYSICS_VERSION, physicsWorld->getFoundation(), physx::PxCookingParams());
  if(!physicsCooking) throw NHException("physics cooker could not be created");

  physicsMaterial = physicsWorld->createMaterial(defaultStaticFriction, defaultDynamicFriction, defaultRestitution);
  if(!physicsMaterial) throw NHException("default physics material could not be created");


  fabrication.setPhysics(getPhysics());
  fabrication.setCooking(getCooking());
  fabrication.setDefaultPhysicsMaterial(getDefaultMaterial());//move all material stuff to the fabrication manager

}

//-------------------------------------------------------------------------------------
PhysicsManager::~PhysicsManager(void)
{
#ifdef _DEBUG
  profileZoneManager->release();
#endif

  physicsMaterial->release();

  physicsCooking->release();
  physicsCooking = NULL;
  
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
FabricationManager* PhysicsManager::getFabrication()
{
  return &fabrication;
}

//-------------------------------------------------------------------------------------
const physx::PxTolerancesScale& PhysicsManager::getTolerancesScale()
{
  return physicsWorld->getTolerancesScale();
}

//-------------------------------------------------------------------------------------
void PhysicsManager::setEnabled(bool enabled)
{
  this->enabled = enabled;
}

//-------------------------------------------------------------------------------------
bool PhysicsManager::isEnabled()
{
  return enabled;
}

