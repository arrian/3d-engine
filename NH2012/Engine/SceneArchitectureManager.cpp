#include "SceneArchitectureManager.h"

#include "Scene.h"
#include "World.h"

#include "SceneGraphicsManager.h"
#include "ScenePhysicsManager.h"
#include "ScenePathfindManager.h"

#include "OgreStaticGeometry.h"

//-------------------------------------------------------------------------------------
SceneArchitectureManager::SceneArchitectureManager(Scene* scene)
  : Identifiable(this, "Main", ARCHITECTURE),
    scene(scene),
    rootNode(scene->getSceneGraphicsManager()->createSceneNode()),
    instanceNumber(0),
    geometry(scene->getSceneGraphicsManager()->createStaticGeometry("architecture")),//potentially unsafe operation... ensure all objects used by getGraphicsManager have been constructed
    nodes(),
    actors(),
    statics(),
    isBuilt(false)
{
  //geometry->setCastShadows(true);//produces no shadows at all?
}

//-------------------------------------------------------------------------------------
SceneArchitectureManager::~SceneArchitectureManager(void)
{
  for(std::vector<physx::PxRigidStatic*>::iterator it = actors.begin(); it != actors.end(); ++it) 
  {
    if(*it) (*it)->release();
    (*it) = NULL;
  }
  
  for(std::vector<Ogre::SceneNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) 
  {
    if(*it) scene->getSceneGraphicsManager()->destroySceneNode(*it);
    (*it) = NULL;
  }
}

//-------------------------------------------------------------------------------------
void SceneArchitectureManager::add(ArchitectureDesc description, Vector3 position, Quaternion quaternion, Vector3 scale)
{
  addStaticTrimesh(description.mesh, description.restitution, description.friction, position, quaternion);
}

//-------------------------------------------------------------------------------------
void SceneArchitectureManager::addStaticTrimesh(std::string meshName, float restitution, float friction, Vector3 position, Quaternion quaternion, Vector3 scale)
{
  Ogre::Entity* entity = scene->getSceneGraphicsManager()->createEntity(meshName);
  if(!entity) throw NHException("could not create architecture entity");

  physx::PxTriangleMesh* mesh = NULL;

  if(statics.count(meshName) > 0) mesh = statics.find(meshName)->second;
  else
  {
    std::cout << "Building " << meshName << "... ";
    mesh = scene->getWorld()->getPhysicsManager()->getFabrication()->createTriangleMeshV2(entity);
    statics.insert(std::pair<Ogre::String,physx::PxTriangleMesh*>(meshName, mesh));
    std::cout << "done." << std::endl;
  }

  if(!mesh) throw NHException("could not create architecture triangle mesh");

  //construct architecture actor
  physx::PxRigidStatic* actor = scene->getWorld()->getPhysicsManager()->getPhysics()->createRigidStatic(physx::PxTransform(physx::PxVec3(position.x, position.y, position.z), physx::PxQuat(quaternion.x,quaternion.y,quaternion.z,quaternion.w)));
  actor->userData = (Identifiable*) this;
  physx::PxShape* shape = actor->createShape(physx::PxTriangleMeshGeometry(mesh), *scene->getWorld()->getPhysicsManager()->getDefaultMaterial());
  if(!shape) throw NHException("could not create architecture physics shape");
  physx::PxFilterData filter;
  filter.word0 = ARCHITECTURE;
  shape->setQueryFilterData(filter);

  scene->getScenePhysicsManager()->addActor(*actor);

  actors.push_back(actor);
  instanceNumber++;

  Ogre::SceneNode* child = rootNode->createChildSceneNode();
  child->setPosition(position);
  child->setOrientation(quaternion);
  child->setScale(scale);
  child->attachObject(entity);

  if(scene->getScenePathfindManager()) scene->getScenePathfindManager()->addGeometry(entity);
}

//-------------------------------------------------------------------------------------
void SceneArchitectureManager::build()
{
  if(isBuilt) throw NHException("attempting to build the architecture multiple times");
  isBuilt = true;

  geometry->addSceneNode(rootNode);
  geometry->build();

  scene->getSceneGraphicsManager()->destroySceneNode(rootNode);
  //is there anything leftover here?
}



