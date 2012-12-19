#pragma once

#include <string>

#include "PhysicsErrorCallback.h"
#include "NHException.h"

//PhysX
#include "PxPhysicsAPI.h"
#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include "cooking/PxCooking.h"
#include "extensions/PxExtensionsAPI.h"
#include "extensions/PxVisualDebuggerExt.h"
#include "PxTkStream.h"


class PhysicsManager
{
public:
  PhysicsManager();
  virtual ~PhysicsManager(void);

  physx::PxCooking* getCooking();
  physx::PxPhysics* getPhysics();
  physx::PxMaterial* getDefaultMaterial();
  physx::PxFoundation* getFoundation();

  const physx::PxTolerancesScale& getTolerancesScale();
  void initialiseVisualDebugger(std::string ip, int port, int timeoutMilliseconds);
private:
  
  //PhysX
  PhysicsErrorCallback errorCallback;
  physx::PxDefaultAllocator allocatorCallback;//for allocating memory to physics manager
  physx::PxFoundation* physicsFoundation;
  physx::PxCooking* physicsCooking;
  physx::PxPhysics* physicsWorld;
  physx::PxMaterial* physicsMaterial;//default material

  //Defaults
  physx::PxReal defaultStaticFriction;
  physx::PxReal defaultDynamicFriction;
  physx::PxReal defaultRestitution;



};

