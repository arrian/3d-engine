#pragma once
#include "NodeComponent.h"

#include "PxPhysicsAPI.h"
#include <OgreEntity.h>

#include "cooking/PxCooking.h"

#include "Vector3.h"
#include "Quaternion.h"

#include "Group.h"


struct PhysicalShape
{
public:
  Vector3 offset;
  Quaternion rotation;
  physx::PxMaterial* material;
  Group group;

  PhysicalShape(Group group, physx::PxMaterial* material, Vector3 offset, Quaternion rotation)
    : group(group),
      material(material),
      offset(offset),
      rotation(rotation)
  {
  }

  virtual void attach(physx::PxRigidActor* actor) = 0;



protected:
  physx::PxTransform getTransform()
  {
    return physx::PxTransform(physx::PxVec3(offset.x, offset.y, offset.z), physx::PxQuat(rotation.x, rotation.y, rotation.z, rotation.w));
  }

  void updateGroup(physx::PxShape* shape)
  {
    physx::PxFilterData filter;
    filter.word0 = group;
    shape->setQueryFilterData(filter);
  }
};

struct ConvexShape : PhysicalShape
{
public:
  physx::PxConvexMesh* mesh;

  ConvexShape(physx::PxConvexMesh* mesh, Group group, physx::PxMaterial* material, Vector3 offset = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY)
    : PhysicalShape(group, material, offset, rotation),
      mesh(mesh)
  {
  }

  virtual void attach(physx::PxRigidActor* actor)
  {
    physx::PxShape* shape = actor->createShape(physx::PxConvexMeshGeometry(mesh), *material, getTransform());//temporary complex collision mesh
    updateGroup(shape);
  }
};

struct BoxShape : PhysicalShape
{
public:
  float length;
  float width;
  float depth;

  BoxShape(float length, float width, float depth, Group group, physx::PxMaterial* material, Vector3 offset = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY)
    : PhysicalShape(group, material, offset, rotation),
      length(length),
      width(width),
      depth(depth)
  {
  }

  virtual void attach(physx::PxRigidActor* actor)
  {
    physx::PxShape* shape = actor->createShape(physx::PxBoxGeometry(length / 2, width / 2, depth / 2), *material, getTransform());
    updateGroup(shape);
  }
};

struct SphereShape : PhysicalShape
{
public:
  float radius;

  SphereShape(float radius, Group group, physx::PxMaterial* material, Vector3 offset = Vector3::ZERO, Quaternion rotation = Quaternion::IDENTITY)
    : PhysicalShape(group, material, offset, rotation),
      radius(radius)
  {
  }

  virtual void attach(physx::PxRigidActor* actor)
  {
    physx::PxShape* shape = actor->createShape(physx::PxSphereGeometry(radius), *material, getTransform());
    updateGroup(shape);
  }
};


class PhysicalComponent : public NodeComponent
{
public:
  PhysicalComponent(Group group, float density = 1.0f);
  ~PhysicalComponent(void);

  void update(double elapsedSeconds);

  void setUserData(void* data);//Provides a 1:1 mapping between the physical representation and the given object.

  physx::PxRigidDynamic* getActor();

  void begin();//begin adding shapes
  void addConvexMesh(physx::PxConvexMesh* mesh, physx::PxMaterial* material = 0, Vector3 offset = Vector3::ZERO);
  void addBoxMesh(float length, float width, float height, physx::PxMaterial* material = 0, Vector3 offset = Vector3::ZERO);
  void addSphereMesh(float radius, physx::PxMaterial* material = 0, Vector3 offset = Vector3::ZERO);
  void end();//end adding shapes

protected:
  bool addingShapes;

  float density;
  Group group;
  physx::PxRigidDynamic* actor;
  std::vector<PhysicalShape*> shapes; 

  void* userData;

  void createActor();
  void hasNodeChange();
};

