#pragma once

//Standard
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>
#include <map>

//Ogre
#include <OgreVector3.h>
#include <OgreString.h>
#include <OgreEntity.h>
#include <OgreSubMesh.h>

//PhysX
#include "PxPhysicsAPI.h"
//#include "extensions/PxDefaultErrorCallback.h"
#include "extensions/PxDefaultAllocator.h"
#include "cooking/PxCooking.h"

/**
 * Handles the physics cooking.
 */
class FabricationManager
{
public:

  ////////////////////////////////////////////////////////////////
  //PhysX Cooking
  class Params
  {
  public:
    Ogre::Vector3 mScale;
    std::map<Ogre::String, physx::PxMaterial*> mMaterialBindings;
    bool mAddBackfaces;

    Params() : mScale(Ogre::Vector3(1,1,1)), mAddBackfaces(false) {}
    ~Params() {}

    Params& scale(const Ogre::Vector3 &scale) { mScale = scale; return *this; }
    Params& materials(std::map<Ogre::String, physx::PxMaterial*> &bindings) { mMaterialBindings = bindings; return *this; }
    Params& backfaces(bool addBackfaces) { mAddBackfaces = addBackfaces; return *this; }
  };

  class AddedMaterials
  {
  public:
    physx::PxMaterial **materials;
    physx::PxU32  materialCount; 

    AddedMaterials() : materials(nullptr) {}
    ~AddedMaterials() { if (materials) delete[] materials; }
  };

  struct MeshInfo
  {
    std::vector<Ogre::Vector3> vertices;//vertex buffer
    std::vector<int> indices;//index buffer
    std::vector<Ogre::String> materials;//assigns a material to each triangle.
  };

  void getMeshInfo(Ogre::MeshPtr mesh, Params &params, MeshInfo &outInfo);
  //void mergeVertices(MeshInfo &outInfo, float fMergeDist = 1e-3f);

  //End PhysX Cooking
  ////////////////////////////////////////////////////////////////////////////////

  FabricationManager();
  virtual ~FabricationManager(void);

  physx::PxConvexMesh* createConvexMesh(const Ogre::MeshPtr& mesh);//Ogre::Entity* e);
  physx::PxTriangleMesh* createTriangleMesh(Ogre::Entity* e);
  physx::PxTriangleMesh* createTriangleMeshV2(Ogre::Entity* e, Params &params = Params(), AddedMaterials *out_addedMaterials = nullptr);

  void setDefaultPhysicsMaterial(physx::PxMaterial* defaultPhysicsMaterial);
  void setCooking(physx::PxCooking* cooking);
  void setPhysics(physx::PxPhysics* physics);
private:
  physx::PxMaterial* material;//default material
  physx::PxCooking* cooking;
  physx::PxPhysics* physics;

  std::map<std::string, physx::PxConvexMesh*> convexHistory;
};

